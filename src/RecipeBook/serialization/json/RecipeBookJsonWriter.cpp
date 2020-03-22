#include "RecipeBookJsonWriter.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "data/Category.h"
#include "data/Ingredient.h"
#include "data/Recipe.h"
#include "data/RecipeItem.h"
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

void json::JsonWriter::writeCategories(const RecipeBook& rRecipeBook, QJsonObject& rRootObject)
{
    QJsonObject object;

    QJsonArray arrayAllCategories;
    for(QString categoryName : rRecipeBook.getAllCategoryNamesSorted())
    {
        arrayAllCategories.append(categoryName);
    }
    object[json::c_strCategoriesAll] = arrayAllCategories;

    QJsonObject objectSortOrders;
    for(QString sortOrderName : rRecipeBook.getAllSortOrderNamesSorted())
    {
        QJsonArray arraySortOrder;
        const SortOrder& rSortOrder = rRecipeBook.getSortOrder(sortOrderName);
        for(uint32_t i = 0; i < rSortOrder.getItemsCount(); ++i)
        {
            arraySortOrder.append(rSortOrder.at(i).getName());
        }

        objectSortOrders[sortOrderName] = arraySortOrder;
    }
    object[json::c_strCategoriesSortOrders] = objectSortOrders;

    rRootObject[json::c_strCategoriesId] = object;
}

void json::JsonWriter::writeIngredients(const RecipeBook& rRecipeBook, QJsonObject& rRootObject)
{
    QJsonObject objectIngredients;
    for(QString ingredientName : rRecipeBook.getAllIngredientNamesSorted())
    {
        QJsonObject ingredientObject;
        const Ingredient& rIngredient = rRecipeBook.getIngredient(ingredientName);
        
        ingredientObject[json::c_strIngredientsCategory] = rIngredient.getCategory().getName();
        if(rIngredient.hasProvenanceEverywhere())
        {
            ingredientObject[json::c_strIngredientsProvenance] = "";
        }
        else
        {
            ingredientObject[json::c_strIngredientsProvenance] = rIngredient.getProvenance().getName();
        }
        ingredientObject[json::c_strIngredientsDefaultUnit] = helper::convertUnit(rIngredient.getDefaultUnit());

        objectIngredients[ingredientName] = ingredientObject;
    }
    
    rRootObject[json::c_strIngredientsId] = objectIngredients;
}

void json::JsonWriter::writeRecipes(const RecipeBook& rRecipeBook, QJsonObject& rRootObject)
{
    QJsonObject objectRecipes;
    for(QString recipeName : rRecipeBook.getAllRecipeNamesSorted())
    {
        QJsonObject recipeObject;
        const Recipe& rRecipe = rRecipeBook.getRecipe(recipeName);
        
        recipeObject[json::c_strRecipesNrPersons] = (int)rRecipe.getNumberOfPersons();

        if(!m_bStripDescriptions)
        {
            recipeObject[json::c_strRecipesShortDesc] = rRecipe.getShortDescription();
            recipeObject[json::c_strRecipesText] = rRecipe.getRecipeText();
            recipeObject[json::c_strRecipesCookingTime] = rRecipe.getCookingTime().toString("HH:mm");
        }

        QJsonObject recipeItemsObject;
        const RecipeItemGroup& rRecipeItems = rRecipe.getRecipeItems();
        writeRecipeItemGroup(rRecipeItems, rRecipeBook, recipeItemsObject);
        recipeObject[json::c_strRecipesItems] = recipeItemsObject;

        QJsonObject itemsGroupsObject;
        for(QString groupName : rRecipe.getAllAlternativesGroupsNamesSorted())
        {
            QJsonObject groupObject;
            const RecipeItemGroup& rRecipeItems = rRecipe.getAlternativesGroup(groupName);
            writeRecipeItemGroup(rRecipeItems, rRecipeBook, groupObject);
            itemsGroupsObject[groupName] = groupObject;
        }
        recipeObject[json::c_strRecipesGroups] = itemsGroupsObject;

        objectRecipes[recipeName] = recipeObject;
    }
    
    rRootObject[json::c_strRecipesId] = objectRecipes;
}

void json::JsonWriter::writeRecipeItemGroup(const RecipeItemGroup& rGroup, const RecipeBook& rRecipeBook, QJsonObject& rObject)
{
    for(QString recipeItemName : rGroup.getAllItemNamesSorted())
    {
        const Ingredient& rIngredient = rRecipeBook.getIngredient(recipeItemName);
        const RecipeItem& rItem = rGroup.getItem(rIngredient);
        QJsonObject recipeItemObject;
        writeRecipeItem(rItem, recipeItemObject);

        rObject[recipeItemName] = recipeItemObject;
    }
}

void json::JsonWriter::writeShoppongList(const RecipeBook& rRecipeBook, QJsonObject& rRootObject)
{
    QJsonObject objectRecipes;
    for(QString recipeName : rRecipeBook.getAllShoppingRecipeNames())
    {
        QJsonObject recipeObject;
        const ShoppingRecipe& rRecipe = rRecipeBook.getShoppingRecipe(recipeName);
        
        recipeObject[json::c_strShoppingRecipesScalingFactor] = rRecipe.getScalingFactor();
        recipeObject[json::c_strShoppingRecipesDueDate] = rRecipe.getDueDate().toString(Qt::ISODate);

        QJsonObject itemsGroupsObject;
        for(QString recipeItemName : rRecipe.getAllItemNamesSorted())
        {
            const Ingredient& rIngredient = rRecipeBook.getIngredient(recipeItemName);
            const ShoppingListItem& rItem = rRecipe.getItem(rIngredient);
            QJsonObject recipeItemObject;

            recipeItemObject[json::c_strRecipesStatus] = helper::convertStatus(rItem.getStatus());
            writeRecipeItem(rItem, recipeItemObject);

            itemsGroupsObject[recipeItemName] = recipeItemObject;
        }
        recipeObject[json::c_strRecipesItems] = itemsGroupsObject;

        objectRecipes[recipeName] = recipeObject;
    }
    
    rRootObject[json::c_strShoppinglistId] = objectRecipes;
}
