#include "RecipeBookJsonReaderV1.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "data/Ingredient.h"
#include "data/Recipe.h"
#include "data/RecipeItem.h"
#include "data/RecipeBook.h"
#include "data/ShoppingRecipe.h"
#include "data/ShoppingListItem.h"
#include "data/SortOrder.h"
#include "../helper/StringConverter.h"
#include "JsonGlobalConstants.h"
#include "JsonFormatConstantsV1.h"

using namespace recipebook;
using namespace recipebook::serialization;

namespace
{
    template<class T> bool readRecipeItem(const QJsonObject& rObject, T& rItem)
    {
        auto iterAmount = rObject.find(json::c_strRecipesAmount);
        if(iterAmount != rObject.end())
        {
            // Old version of Amount without min / max.

            QJsonArray amount = rObject[json::c_strRecipesAmountMinMax].toArray();
            if(amount.size() != 2)
            {
                return false;
            }

            double dMin = amount[0].toDouble();
            QString strUnit = amount[1].toString();

            rItem.getAmount().setIsRange(false);
            rItem.getAmount().setQuantityMin(dMin);

            Unit unit = helper::convertUnit(strUnit);
            rItem.getAmount().setUnit(unit);
        }
        else
        {
            QJsonArray amountMinMax = rObject[json::c_strRecipesAmountMinMax].toArray();
            if(amountMinMax.size() != 3)
            {
                return false;
            }

            double dMin = amountMinMax[0].toDouble();
            double dMax = amountMinMax[1].toDouble();
            QString strUnit = amountMinMax[2].toString();

            bool bRange = dMax >= 0.0f;
            rItem.getAmount().setIsRange(bRange);
            rItem.getAmount().setQuantityMin(dMin);
            if(bRange)
            {
                rItem.getAmount().setQuantityMax(dMax);
            }

            Unit unit = helper::convertUnit(strUnit);
            rItem.getAmount().setUnit(unit);
        }

        QString strSize = rObject[json::c_strRecipesSize].toString();
        Size size = helper::convertSize(strSize);
        rItem.setSize(size);

        bool bOptional = rObject[json::c_strRecipesOptional].toBool();
        rItem.setIsOptional(bOptional);

        QString strAdditionalInfo = rObject[json::c_strRecipesAdditionalInfo].toString();
        rItem.setAdditionInfo(strAdditionalInfo);

        return true;
    }
}

json::JsonReaderV1::JsonReaderV1()
{
}

bool json::JsonReaderV1::read(const QJsonArray& rRootArray, RBMetaData& rMetaData, RecipeBook& rRecipeBook)
{
    if(rRootArray.size() != 5)
    {
        qWarning("Invalid file format.");
        return false;
    }

    if(!readMetadata(rRootArray[0].toObject(), rMetaData))
    {
        qWarning("Invalid metadata.");
        return false;
    }

    rRecipeBook.clearData();
    
    if(!readCategories(rRootArray[1].toObject(), rRecipeBook))
    {
        qWarning("Invalid categories.");
        rRecipeBook.clearData();
        return false;
    }

    if(!readIngredients(rRootArray[2].toObject(), rRecipeBook))
    {
        qWarning("Invalid ingredients.");
        rRecipeBook.clearData();
        return false;
    }

    if(!readRecipes(rRootArray[3].toObject(), rRecipeBook))
    {
        qWarning("Invalid recipes.");
        rRecipeBook.clearData();
        return false;
    }

    if(!readShoppingList(rRootArray[4].toObject(), rRecipeBook))
    {
        qWarning("Invalid shoppinglist.");
        rRecipeBook.clearData();
        return false;
    }

	return true;
}

bool json::JsonReaderV1::readMetadata(const QJsonObject& rObject, RBMetaData& rMetaData)
{
    QString strSerializerID = rObject[json::c_strId].toString();
    if(strSerializerID != c_strProgramId)
    {
        qWarning("Invalid id " + strSerializerID.toLatin1() + ".");
        return false;
    }

    QJsonArray originArray = rObject[json::c_strOriginId].toArray();
    if(originArray.size() != 2)
    {
        qWarning("Invalid origin");
        return false;
    }
    rMetaData.strOrigin = originArray[0].toString();
    rMetaData.strUID = originArray[1].toString();

    int iVersion = rObject[json::c_strVersion].toInt();
    if(iVersion > c_uiSerializerVersion || iVersion < 0)
    {
        qWarning("Invalid version.");
        return false;
    }

    return true;
}

bool json::JsonReaderV1::readCategories(const QJsonObject& rObject, RecipeBook& rRecipeBook)
{
    QString id = rObject[json::c_strId].toString();
    if(id != json::c_strCategoriesId)
    {
        qWarning("Invalid categories id " + id.toLatin1() + ".");
        return false;
    }
    
    QJsonArray allCategories = rObject[json::c_strCategoriesAll].toArray();
    for(auto category : qAsConst(allCategories))
    {
        rRecipeBook.addCategory(category.toString());
    }

    QJsonObject allSortOrders = rObject[json::c_strCategoriesSortOrders].toObject();
    for(QString sortOrderName : allSortOrders.keys())
    {
        SortOrder& rSortOrder = rRecipeBook.addSortOrder(sortOrderName);
        QJsonArray order = allSortOrders[sortOrderName].toArray();
        for(int i = 0; i < order.size(); ++i)
        {
            QString categoryName = order[i].toString();
            rSortOrder.moveCategory(rRecipeBook.getCategory(categoryName), i);
        }
    }

    return true;
}

bool json::JsonReaderV1::readIngredients(const QJsonObject& rObject, RecipeBook& rRecipeBook)
{
    QString id = rObject[json::c_strId].toString();
    if(id != json::c_strIngredientsId)
    {
        qWarning("Invalid ingredients id " + id.toLatin1() + ".");
        return false;
    }

    for(QString strIngredientName : rObject.keys())
    {
        if(strIngredientName == json::c_strId)
        {
            continue;
        }

        QJsonObject ingredient = rObject[strIngredientName].toObject();
        QString strCategory = ingredient[json::c_strIngredientsCategory].toString();
        QString strProvenance = ingredient[json::c_strIngredientsProvenance].toString();
        QString strDefaultUnit = ingredient[json::c_strIngredientsDefaultUnit].toString();

        const Category& rCategory = rRecipeBook.getCategory(strCategory);
        Unit defaultUnit = helper::convertUnit(strDefaultUnit);
        SortOrder* pProvenance = nullptr;

        Ingredient& rIngredient = rRecipeBook.addIngredient(strIngredientName, rCategory, defaultUnit);
        if(strProvenance != json::c_strProvenanceEverywhere)
        {
            rIngredient.setProvenance(rRecipeBook.getSortOrder(strProvenance));
        }
    }

    return true;
}

bool json::JsonReaderV1::readRecipes(const QJsonObject& rObject, RecipeBook& rRecipeBook)
{
    QString id = rObject[json::c_strId].toString();
    if(id != json::c_strRecipesId)
    {
        qWarning("Invalid recipes id " + id.toLatin1() + ".");
        return false;
    }

    for(QString strRecipeName : rObject.keys())
    {
        if(strRecipeName == json::c_strId
           || strRecipeName == json::c_strRecipesActiveRecipe)
        {
            continue;
        }

        QJsonObject recipe = rObject[strRecipeName].toObject();
        uint32_t uiNrPersons = recipe[json::c_strRecipesNrPersons].toInt();

        Recipe& rRecipe = rRecipeBook.addRecipe(strRecipeName, uiNrPersons);
        for(QString strRecipeItem : recipe.keys())
        {
            if(strRecipeItem == json::c_strRecipesNrPersons)
            {
                continue;
            }

            if(strRecipeItem == json::c_strRecipesGroup)
            {
                if(!readRecipeItemGroup(recipe[strRecipeItem].toObject(), rRecipeBook, rRecipe))
                {
                    return false;
                }
            }
            else
            {
                const Ingredient& rIngredient = rRecipeBook.getIngredient(strRecipeItem);
                RecipeItem& rItem = rRecipe.addRecipeItem(rIngredient);
                if(!readRecipeItem(recipe[strRecipeItem].toObject(), rItem))
                {
                    return false;
                }
            }
        }
    }

    return true;
}

bool json::JsonReaderV1::readRecipeItemGroup(const QJsonObject& rObject, RecipeBook& rRecipeBook, Recipe& rRecipe)
{
    QString groupName = rObject[json::c_strRecipesGroupName].toString();

    for(QString strItemName : rObject.keys())
    {
        if(strItemName == json::c_strRecipesGroupName)
        {
            continue;
        }

        const Ingredient& rIngredient = rRecipeBook.getIngredient(strItemName);
        RecipeItem& rItem = rRecipe.addRecipeItem(rIngredient);

        if(groupName != "")
        {
            if(!rRecipeBook.existsAlternativesType(groupName))
            {
                rRecipeBook.addAlternativesType(groupName);
            }
            rItem.setAlternativesGroup(rRecipeBook.getAlternativesType(groupName));
        }

        if(!readRecipeItem(rObject[strItemName].toObject(), rItem))
        {
            return false;
        }
    }

    return true;
}

bool json::JsonReaderV1::readShoppingList(const QJsonObject& rObject, RecipeBook& rRecipeBook)
{
    QString id = rObject[json::c_strId].toString();
    if(id != json::c_strShoppinglistId)
    {
        qWarning("Invalid shoppinglist id " + id.toLatin1() + ".");
        return false;
    }

    for(QString strRecipeName : rObject.keys())
    {
        if(strRecipeName == json::c_strId
           || strRecipeName == json::c_strShoppinglistSortOrder)
        {
            continue;
        }

        QJsonObject recipe = rObject[strRecipeName].toObject();
        float fScalingFactor = recipe[json::c_strShoppingRecipesScalingFactor].toDouble();
        QString strDueDate = recipe[json::c_strShoppingRecipesDueDate].toString();

        ShoppingRecipe& rRecipe = rRecipeBook.addNewShoppingRecipe(strRecipeName, fScalingFactor);

        rRecipe.setDueDate(QDate::fromString(strDueDate, Qt::ISODate));

        for(QString strRecipeItem : recipe.keys())
        {
            if(strRecipeItem == json::c_strShoppingRecipesScalingFactor
               || strRecipeItem == json::c_strShoppingRecipesDueDate)
            {
                continue;
            }

            const Ingredient& rIngredient = rRecipeBook.getIngredient(strRecipeItem);
            ShoppingListItem& rItem = rRecipe.addItem(rIngredient);

            QJsonObject item = recipe[strRecipeItem].toObject();

            QString strStatus = item[json::c_strRecipesStatus].toString();
            Status status = helper::convertStatus(strStatus);
            rItem.setStatus(status);

            if(!readRecipeItem(item, rItem))
            {
                return false;
            }
        }
    }

    return true;
}
