#include "RecipeBookJsonWriter.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "data/Category.h"
#include "data/Ingredient.h"
#include "data/Recipe.h"
#include "data/RecipeItem.h"
#include "data/AlternativesType.h"
#include "data/RecipeBook.h"
#include "data/ShoppingRecipe.h"
#include "data/ShoppingListItem.h"
#include "data/SortOrder.h"
#include "../helper/StringConverter.h"
#include "JsonGlobalConstants.h"
#include "JsonFormatConstantsV2.h"

using namespace recipebook;
using namespace recipebook::serialization;

namespace
{
    template<class T> void writeRecipeItem(const T& rItem, QJsonObject& rObject)
    {
        QJsonObject amountObject;
        amountObject[json::c_strRecipesAmountMin] = rItem.getAmount().getQuantityMin();
        amountObject[json::c_strRecipesAmountMax] = rItem.getAmount().isRange() ? rItem.getAmount().getQuantityMax() : -1.0f;
        amountObject[json::c_strRecipesAmountUnit] = helper::convertUnit(rItem.getAmount().getUnit());
        rObject[json::c_strRecipesAmount] = amountObject;

        rObject[json::c_strRecipesSize] = helper::convertSize(rItem.getSize());
        rObject[json::c_strRecipesOptional] = rItem.isOptional();
        rObject[json::c_strRecipesAdditionalInfo] = rItem.getAdditionalInfo();

        if(rItem.hasAlternativesGroup())
        {
            rObject[json::c_strRecipesGroup] = rItem.getAlternativesGroup().getName();
        }
        else
        {
            rObject[json::c_strRecipesGroup] = "";
        }
    }
}

json::JsonWriter::JsonWriter(QString strUID, bool bStripDescriptions)
:   m_strUID(strUID),
    m_bStripDescriptions(bStripDescriptions)
{
}

bool json::JsonWriter::serialize(const RecipeBook& rRecipeBook, QFile& file)
{
    RBMetaData metaData;
    metaData.strOrigin = json::c_strProgramType;
    metaData.strUID = m_strUID;

	QJsonObject rootObject;

    writeMetadata(metaData, rootObject);

    writeCategories(rRecipeBook, rootObject);
    writeIngredients(rRecipeBook, rootObject);
    writeAlternativesTypes(rRecipeBook, rootObject);
    writeRecipes(rRecipeBook, rootObject);
    writeShoppongList(rRecipeBook, rootObject);

    QJsonDocument jsonDoc(rootObject);

	if (!file.open(QIODevice::WriteOnly))
	{
        qWarning("Couldn't open save file.");
        return false;
    }

	file.write(jsonDoc.toJson());

    file.close();

	return true;
}

void json::JsonWriter::writeMetadata(const RBMetaData& rMetaData, QJsonObject& rRootObject)
{
    QJsonObject object;

    object[json::c_strMetaDataId] = json::c_strProgramId;
    object[json::c_strOriginSW] = rMetaData.strOrigin;
    object[json::c_strOriginUID] = rMetaData.strUID;
    object[json::c_strVersion] = (int)json::c_uiSerializerVersion;
        
    rRootObject[json::c_strMetadataId] = object;
}

void json::JsonWriter::writeAlternativesTypes(const RecipeBook& rRecipeBook, QJsonObject& rRootObject)
{
    QJsonObject objectAlternativesTypes;
    for(quint32 i = 0; i < rRecipeBook.getAlternativesTypesCount(); ++i)
    {
        QJsonObject alternativesType;
        const AlternativesType& rType = rRecipeBook.getAlternativesTypeAt(i);
        
        alternativesType[c_strAlternativesColor] = rType.getColor().name();

        objectAlternativesTypes[rType.getName()] = alternativesType;
    }
    
    rRootObject[json::c_strAlternativesTypesId] = objectAlternativesTypes;
}

void json::JsonWriter::writeCategories(const RecipeBook& rRecipeBook, QJsonObject& rRootObject)
{
    QJsonObject object;

    QJsonArray arrayAllCategories;
    for(quint32 i = 0; i < rRecipeBook.getCategoriesCount(); ++i)
    {
        arrayAllCategories.append(rRecipeBook.getCategoryAt(i).getName());
    }
    object[json::c_strCategoriesAll] = arrayAllCategories;

    QJsonObject objectSortOrders;
    for(quint32 j = 0; j < rRecipeBook.getSortOrdersCount(); ++j)
    {
        QJsonArray arraySortOrder;
        const SortOrder& rSortOrder = rRecipeBook.getSortOrderAt(j);
        for(uint32_t i = 0; i < rSortOrder.getItemsCount(); ++i)
        {
            arraySortOrder.append(rSortOrder.getItemAt(i).getName());
        }

        objectSortOrders[rSortOrder.getName()] = arraySortOrder;
    }
    object[json::c_strCategoriesSortOrders] = objectSortOrders;

    rRootObject[json::c_strCategoriesId] = object;
}

void json::JsonWriter::writeIngredients(const RecipeBook& rRecipeBook, QJsonObject& rRootObject)
{
    QJsonObject objectIngredients;
    for(quint32 i = 0; i < rRecipeBook.getIngredientsCount(); ++i)
    {
        QJsonObject ingredientObject;
        const Ingredient& rIngredient = rRecipeBook.getIngredientAt(i);
        
        ingredientObject[json::c_strIngredientsCategory] = rIngredient.getCategory().getName();

		QJsonArray unavailableProvenances;
		for(quint32 i = 0; i < rRecipeBook.getSortOrdersCount(); ++i)
		{
			const SortOrder& rOrder = rRecipeBook.getSortOrderAt(i);
			if(!rIngredient.provenanceAvailable(rOrder))
			{
				unavailableProvenances.append(rOrder.getName());
			}
		}
		ingredientObject[json::c_strIngredientsUnavailProvenances] = unavailableProvenances;

        ingredientObject[json::c_strIngredientsDefaultUnit] = helper::convertUnit(rIngredient.getDefaultUnit());

        objectIngredients[rIngredient.getName()] = ingredientObject;
    }
    
    rRootObject[json::c_strIngredientsId] = objectIngredients;
}

void json::JsonWriter::writeRecipes(const RecipeBook& rRecipeBook, QJsonObject& rRootObject)
{
    QJsonObject objectRecipes;
    for(quint32 i = 0; i < rRecipeBook.getRecipesCount(); ++i)
    {
        QJsonObject recipeObject;
        const Recipe& rRecipe = rRecipeBook.getRecipeAt(i);
        
        recipeObject[json::c_strRecipesNrPersons] = (int)rRecipe.getNumberOfPersons();
        
        if(!m_bStripDescriptions)
        {
            recipeObject[json::c_strRecipesShortDesc] = rRecipe.getShortDescription();
            recipeObject[json::c_strRecipesText] = rRecipe.getRecipeText();
            recipeObject[json::c_strRecipesCookingTime] = rRecipe.getCookingTime().toString("HH:mm");
        }

        QJsonObject recipeItemsObject;
        writeRecipeItems(rRecipe, recipeItemsObject);
        recipeObject[json::c_strRecipesItems] = recipeItemsObject;

        objectRecipes[rRecipe.getName()] = recipeObject;
    }
    
    rRootObject[json::c_strRecipesId] = objectRecipes;
}

void json::JsonWriter::writeRecipeItems(const Recipe& rRecipe, QJsonObject& rObject)
{
    for(quint32 i = 0; i < rRecipe.getRecipeItemsCount(); ++i)
    {
        const RecipeItem& rItem = rRecipe.getRecipeItemAt(i);
        
        QJsonObject recipeItemObject;
        recipeItemObject[json::c_strRecipesPosition] = (int)i;
        writeRecipeItem(rItem, recipeItemObject);

        rObject[rItem.getName()] = recipeItemObject;
    }
}

void json::JsonWriter::writeShoppongList(const RecipeBook& rRecipeBook, QJsonObject& rRootObject)
{
    QJsonObject objectRecipes;
    for(quint32 j = 0; j < rRecipeBook.getShoppingRecipesCount(); ++j)
    {
        QJsonObject recipeObject;
        const ShoppingRecipe& rRecipe = rRecipeBook.getShoppingRecipeAt(j);
        
        recipeObject[json::c_strShoppingRecipesScalingFactor] = rRecipe.getScalingFactor();
        recipeObject[json::c_strShoppingRecipesDueDate] = rRecipe.getDueDate().toString(Qt::ISODate);

        QJsonObject itemsGroupsObject;
        for(quint32 i = 0; i < rRecipe.getItemsCount(); ++i)
        {
            const ShoppingListItem& rItem = rRecipe.getItemAt(i);
            QJsonObject recipeItemObject;

            recipeItemObject[json::c_strRecipesStatus] = helper::convertStatus(rItem.getStatus());
			recipeItemObject[json::c_strRecipesItemEnabled] = rItem.isItemEnabled();
			
            writeRecipeItem(rItem, recipeItemObject);

            itemsGroupsObject[rItem.getName()] = recipeItemObject;
        }
        recipeObject[json::c_strRecipesItems] = itemsGroupsObject;

        objectRecipes[rRecipe.getName()] = recipeObject;
    }
    
    rRootObject[json::c_strShoppinglistId] = objectRecipes;
}
