#include "RecipeBookJsonReaderV2.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMap>
#include "data/AlternativesType.h"
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
	template<class T> bool readRecipeItem(const QJsonObject& rObject, const RecipeBook& rRecipeBook, T& rItem)
	{
		// Amount
		QJsonObject amountMinMax = rObject[json::c_strRecipesAmount].toObject();
		double dMin = amountMinMax[json::c_strRecipesAmountMin].toDouble();
		double dMax = amountMinMax[json::c_strRecipesAmountMax].toDouble();
		QString strUnit = amountMinMax[json::c_strRecipesAmountUnit].toString();

		bool bRange = dMax >= 0.0f;
		rItem.getAmount().setIsRange(bRange);
		rItem.getAmount().setQuantityMin(dMin);
		if(bRange)
		{
			rItem.getAmount().setQuantityMax(dMax);
		}
		Unit unit = serialization::helper::convertUnit(strUnit);
		rItem.getAmount().setUnit(unit);

		// Size
		QString strSize = rObject[json::c_strRecipesSize].toString();
		Size size = serialization::helper::convertSize(strSize);
		rItem.setSize(size);

		// Optional
		bool bOptional = rObject[json::c_strRecipesOptional].toBool();
		rItem.setIsOptional(bOptional);

		// Additional info
		QString strAdditionalInfo = rObject[json::c_strRecipesAdditionalInfo].toString();
		rItem.setAdditionInfo(strAdditionalInfo);

		// Alternatives group
		QString groupName = rObject[json::c_strRecipesGroup].toString();
		if(groupName != "")
		{
			rItem.setAlternativesGroup(rRecipeBook.getAlternativesType(groupName));
		}

		return true;
	}
}

json::JsonReaderV2::JsonReaderV2()
{
}

bool json::JsonReaderV2::read(QJsonObject& rRootObject, RBMetaData& rMetaData, RecipeBook& rRecipeBook)
{
	if(rRootObject.size() != 6)
	{
		qWarning("Invalid file format.");
		return false;
	}

	if(!readMetadata(rRootObject[json::c_strMetadataId].toObject(), rMetaData))
	{
		qWarning("Invalid metadata.");
		return false;
	}

	rRecipeBook.clearData();
    
	if(!readCategories(rRootObject[json::c_strCategoriesId].toObject(), rRecipeBook))
	{
		qWarning("Invalid categories.");
		rRecipeBook.clearData();
		return false;
	}

	if(!readIngredients(rRootObject[json::c_strIngredientsId].toObject(), rRecipeBook))
	{
		qWarning("Invalid ingredients.");
		rRecipeBook.clearData();
		return false;
	}

	if(!readAlternativesTypes(rRootObject[json::c_strAlternativesTypesId].toObject(), rRecipeBook))
	{
		qWarning("Invalid alternatives types.");
		rRecipeBook.clearData();
		return false;
	}

	if(!readRecipes(rRootObject[json::c_strRecipesId].toObject(), rRecipeBook))
	{
		qWarning("Invalid recipes.");
		rRecipeBook.clearData();
		return false;
	}

	if(!readShoppingList(rRootObject[json::c_strShoppinglistId].toObject(), rRecipeBook))
	{
		qWarning("Invalid shoppinglist.");
		rRecipeBook.clearData();
		return false;
	}

	return true;
}

bool json::JsonReaderV2::readMetadata(const QJsonObject& rObject, RBMetaData& rMetaData)
{
	QString strSerializerID = rObject[json::c_strMetaDataId].toString();
	if(strSerializerID != c_strProgramId)
	{
		qWarning("Invalid id " + strSerializerID.toLatin1() + ".");
		return false;
	}

	rMetaData.strOrigin = rObject[json::c_strOriginSW].toString();
	rMetaData.strUID = rObject[json::c_strOriginUID].toString();

	int iVersion = rObject[json::c_strVersion].toInt();
	if(iVersion > c_uiSerializerVersion || iVersion < 0)
	{
		qWarning("Invalid version.");
		return false;
	}

	return true;
}

bool json::JsonReaderV2::readAlternativesTypes(const QJsonObject& rObject, RecipeBook& rRecipeBook)
{
	for(QString strAlternativesTypeName : rObject.keys())
	{
		QJsonObject alternativesType = rObject[strAlternativesTypeName].toObject();
		QString strColor = alternativesType[json::c_strAlternativesColor].toString();
        
		AlternativesType& rType = rRecipeBook.addAlternativesType(strAlternativesTypeName);
		rType.setColor(strColor);
	}

	return true;
}

bool json::JsonReaderV2::readCategories(const QJsonObject& rObject, RecipeBook& rRecipeBook)
{
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

bool json::JsonReaderV2::readIngredients(const QJsonObject& rObject, RecipeBook& rRecipeBook)
{
	for(QString strIngredientName : rObject.keys())
	{
		QJsonObject ingredient = rObject[strIngredientName].toObject();
		QString strCategory = ingredient[json::c_strIngredientsCategory].toString();
		QJsonArray unavailableProvenances = ingredient[json::c_strIngredientsUnavailProvenances].toArray();
		QString strDefaultUnit = ingredient[json::c_strIngredientsDefaultUnit].toString();

		const Category& rCategory = rRecipeBook.getCategory(strCategory);
		Unit defaultUnit = helper::convertUnit(strDefaultUnit);

		Ingredient& rIngredient = rRecipeBook.addIngredient(strIngredientName, rCategory, defaultUnit);
		for(auto strProvenance : qAsConst(unavailableProvenances))
		{
			const SortOrder& rOrder = rRecipeBook.getSortOrder(strProvenance.toString());
			rIngredient.setProvenanceUnavailable(rOrder);
		}
	}

	return true;
}

bool json::JsonReaderV2::readRecipes(const QJsonObject& rObject, RecipeBook& rRecipeBook)
{
	for(QString strRecipeName : rObject.keys())
	{
		QJsonObject recipe = rObject[strRecipeName].toObject();
		uint32_t uiNrPersons = recipe[json::c_strRecipesNrPersons].toInt();

		Recipe& rRecipe = rRecipeBook.addRecipe(strRecipeName, uiNrPersons);

		if(recipe.find(json::c_strRecipesShortDesc) != recipe.end())
		{
			rRecipe.setShortDescription(recipe[json::c_strRecipesShortDesc].toString());
		}
		if(recipe.find(json::c_strRecipesText) != recipe.end())
		{
			rRecipe.setRecipeText(recipe[json::c_strRecipesText].toString());
		}
		if(recipe.find(json::c_strRecipesCookingTime) != recipe.end())
		{
			QString strTime = recipe[json::c_strRecipesCookingTime].toString();
			rRecipe.setCookingTime(QTime::fromString(strTime, "HH:mm"));
		}

		QJsonObject items = recipe[json::c_strRecipesItems].toObject();
		QMap<int, RecipeItem*> indexMap;
		for(QString strRecipeItem : items.keys())
		{
			QJsonObject item = items[strRecipeItem].toObject();
			int posItem = item[json::c_strRecipesPosition].toInt();

			const Ingredient& rIngredient = rRecipeBook.getIngredient(strRecipeItem);
			RecipeItem& rItem = rRecipe.addRecipeItem(rIngredient);
			indexMap.insert(posItem, &rItem);

			QJsonObject itemObject = items[strRecipeItem].toObject();
			if(!readRecipeItem(itemObject, rRecipeBook, rItem))
			{
				return false;
			}
		}

		// Sort items correctly
		for(int i : indexMap.keys())
		{
			rRecipe.moveRecipeItem(*indexMap[i], i);
		}
		if(m_bUseAlternativesGroupsSorting)
		{
			rRecipe.moveGroupItemsTogether();
		}
	}

	return true;
}

bool json::JsonReaderV2::readShoppingList(const QJsonObject& rObject, RecipeBook& rRecipeBook)
{
	for(QString strRecipeName : rObject.keys())
	{
		QJsonObject recipe = rObject[strRecipeName].toObject();
		float fScalingFactor = recipe[json::c_strShoppingRecipesScalingFactor].toDouble();
		QString strDueDate = recipe[json::c_strShoppingRecipesDueDate].toString();

		ShoppingRecipe& rRecipe = rRecipeBook.addNewShoppingRecipe(strRecipeName, fScalingFactor);
		rRecipe.setDueDate(QDate::fromString(strDueDate, Qt::ISODate));

		QJsonObject items = recipe[json::c_strRecipesItems].toObject();
		QMap<int, ShoppingListItem*> indexMap;
		for(QString strRecipeItem : items.keys())
		{
			QJsonObject item = items[strRecipeItem].toObject();

			const Ingredient& rIngredient = rRecipeBook.getIngredient(strRecipeItem);
			ShoppingListItem& rItem = rRecipe.addItem(rIngredient);

			QString strStatus = item[json::c_strRecipesStatus].toString();
			Status status = helper::convertStatus(strStatus);
			rItem.setStatus(status);

			bool bItemEnabled = item[json::c_strRecipesItemEnabled].toBool(true);
			rItem.setItemEnabled(bItemEnabled);

			if(!readRecipeItem(item, rRecipeBook, rItem))
			{
				return false;
			}
		}
	}

	return true;
}
