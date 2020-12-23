#include "RecipeBookChanges.h"
#include <QException>
#include <data/RecipeBook.h>
#include <data/Category.h>
#include <data/SortOrder.h>
#include <data/AlternativesType.h>
#include <data/Ingredient.h>
#include <data/Recipe.h>
#include <data/RecipeItem.h>
#include <data/RecipeBookConfiguration.h>
#include <data/RecipeBookConfigItem.h>

using namespace recipebook;

RecipeBookChanges::RecipeBookChanges()
{
}

void RecipeBookChanges::compare(const QSharedPointer<RecipeBook> spBase, const QSharedPointer<RecipeBook> spChanged)
{
	clear();

	compareCategories(spBase, spChanged);
}

void RecipeBookChanges::clear()
{
	clearChanges(m_CategoryChanges);
	clearChanges(m_SortOrderChanges);
	clearChanges(m_AlternativesTypesChanges);
	clearChanges(m_IngredientsChanges);

	clearChanges(m_RecipesChanges);
	m_RecipeItemChanges.clear();

	clearChanges(m_RBConfigurationsChanges);
	m_RBConfigItemChanges.clear();
}

void RecipeBookChanges::compareCategories(const QSharedPointer<RecipeBook> spBase, const QSharedPointer<RecipeBook> spChanged)
{
	// Find added categories
	for(quint32 i = 0; i < spChanged->getCategoriesCount(); ++i)
	{
		QString strCategory = spChanged->getCategoryAt(i).getName();
		if(!spBase->existsCategory(strCategory))
		{
			m_CategoryChanges.m_Added.push_back(strCategory);
		}
	}

	// Find removed categories
	for(quint32 i = 0; i < spBase->getCategoriesCount(); ++i)
	{
		QString strCategory = spBase->getCategoryAt(i).getName();
		if(!spChanged->existsCategory(strCategory))
		{
			m_CategoryChanges.m_Removed.push_back(strCategory);
		}
	}
}

void RecipeBookChanges::compareSortOrders(const QSharedPointer<RecipeBook> spBase, const QSharedPointer<RecipeBook> spChanged)
{
	// Verify changed and added sort orders
	for(quint32 i = 0; i < spChanged->getSortOrdersCount(); ++i)
	{
		QString strSortOrder = spChanged->getSortOrderAt(i).getName();
		if(!spBase->existsSortOrder(strSortOrder))
		{
			// Sort order added
			m_SortOrderChanges.m_Added.append(strSortOrder);
		}
		else
		{
			// Sort order changed?
			const SortOrder& rOrderChanged = spChanged->getSortOrderAt(i);
			const SortOrder& rOrderBase = spBase->getSortOrder(strSortOrder);
			if(rOrderChanged != rOrderBase)
			{
				m_SortOrderChanges.m_Changed.append(strSortOrder);
			}
		}
	}

	// Find removed categories
	for(quint32 i = 0; i < spBase->getSortOrdersCount(); ++i)
	{
		QString strSortOrder = spBase->getSortOrderAt(i).getName();
		if(!spChanged->existsSortOrder(strSortOrder))
		{
			m_SortOrderChanges.m_Removed.push_back(strSortOrder);
		}
	}
}

void RecipeBookChanges::compareAlternativesTypes(const QSharedPointer<RecipeBook> spBase, const QSharedPointer<RecipeBook> spChanged)
{
	// Verify changed and added alternatives types
	for(quint32 i = 0; i < spChanged->getAlternativesTypesCount(); ++i)
	{
		QString strAlternativesTypes = spChanged->getAlternativesTypeAt(i).getName();
		if(!spBase->existsAlternativesType(strAlternativesTypes))
		{
			// Alternatives type added
			m_AlternativesTypesChanges.m_Added.push_back(strAlternativesTypes);
		}
		else
		{
			// Alternatives type changed?
			const AlternativesType& rTypeChanged = spChanged->getAlternativesTypeAt(i);
			const AlternativesType& rTypeBase = spBase->getAlternativesType(strAlternativesTypes);
			if(rTypeChanged != rTypeBase)
			{
				m_AlternativesTypesChanges.m_Changed.append(strAlternativesTypes);
			}
		}
	}

	// Find removed alternatives types
	for(quint32 i = 0; i < spBase->getAlternativesTypesCount(); ++i)
	{
		QString strAlternativesTypes = spBase->getAlternativesTypeAt(i).getName();
		if(!spChanged->existsAlternativesType(strAlternativesTypes))
		{
			m_AlternativesTypesChanges.m_Removed.push_back(strAlternativesTypes);
		}
	}
}

void RecipeBookChanges::compareIngredients(const QSharedPointer<RecipeBook> spBase, const QSharedPointer<RecipeBook> spChanged)
{
	// Verify changed and added ingredients
	for(quint32 i = 0; i < spChanged->getIngredientsCount(); ++i)
	{
		QString strIngredient = spChanged->getIngredientAt(i).getName();
		if(!spBase->existsIngredient(strIngredient))
		{
			// Ingredients added
			m_IngredientsChanges.m_Added.push_back(strIngredient);
		}
		else
		{
			// Ingredients changed?
			const Ingredient& rChanged = spChanged->getIngredientAt(i);
			const Ingredient& rBase = spBase->getIngredient(strIngredient);
			if(rChanged != rBase)
			{
				m_IngredientsChanges.m_Changed.append(strIngredient);
			}
		}
	}

	// Find removed ingredients
	for(quint32 i = 0; i < spBase->getIngredientsCount(); ++i)
	{
		QString strIngredient = spBase->getIngredientAt(i).getName();
		if(!spChanged->existsIngredient(strIngredient))
		{
			m_IngredientsChanges.m_Removed.push_back(strIngredient);
		}
	}
}

void RecipeBookChanges::compareRecipes(const QSharedPointer<RecipeBook> spBase, const QSharedPointer<RecipeBook> spChanged)
{
	// Verify changed and added recipes
	for(quint32 i = 0; i < spChanged->getRecipesCount(); ++i)
	{
		QString strRecipe = spChanged->getRecipeAt(i).getName();
		if(!spBase->existsRecipe(strRecipe))
		{
			// Recipes added
			m_RecipesChanges.m_Added.push_back(strRecipe);
		}
		else
		{
			// Recipe changed?
			const Recipe& rChanged = spChanged->getRecipeAt(i);
			const Recipe& rBase = spBase->getRecipe(strRecipe);
			bool bChanged = compareRecipeItems(strRecipe, rBase, rChanged, spBase, spChanged);
			if(bChanged || rChanged != rBase)
			{
				m_RecipesChanges.m_Changed.append(strRecipe);
			}
		}
	}

	// Find removed recipes
	for(quint32 i = 0; i < spBase->getRecipesCount(); ++i)
	{
		QString strRecipe = spBase->getRecipeAt(i).getName();
		if(!spChanged->existsRecipe(strRecipe))
		{
			m_RecipesChanges.m_Removed.push_back(strRecipe);
		}
	}
}

bool RecipeBookChanges::compareRecipeItems(QString strRecipe,
										   const Recipe& rRecipeBase, 
										   const Recipe& rRecipeChanged, 
										   const QSharedPointer<RecipeBook> spBase,
										   const QSharedPointer<RecipeBook> spChanged)
{
	bool bChanged = false;

	// Verify changed and added recipe items
	for(quint32 i = 0; i < rRecipeChanged.getRecipeItemsCount(); ++i)
	{
		QString strRecipeItem = rRecipeChanged.getRecipeItemAt(i).getName();
		if(spBase->existsIngredient(strRecipeItem))
		{
			// (New) recipe item added
			m_RecipeItemChanges[strRecipe].m_Added.push_back(strRecipeItem);
			bChanged = true;
			continue;
		}
		const Ingredient& rIngredientBase = spBase->getIngredient(strRecipeItem);
		if(!rRecipeBase.existsRecipeItem(rIngredientBase))
		{
			// Recipe item added
			m_RecipeItemChanges[strRecipe].m_Added.push_back(strRecipeItem);
			bChanged = true;
		}
		else
		{
			// Recipe item changed?
			const RecipeItem& rItemChanged = rRecipeChanged.getRecipeItemAt(i);
			const RecipeItem& rItemBase = rRecipeBase.getRecipeItem(rIngredientBase);
			if(rItemChanged != rItemBase)
			{
				m_RecipeItemChanges[strRecipe].m_Changed.push_back(strRecipeItem);
				bChanged = true;
			}
		}
	}

	// Find removed recipes
	for(quint32 i = 0; i < rRecipeBase.getRecipeItemsCount(); ++i)
	{
		QString strRecipeItem = rRecipeBase.getRecipeItemAt(i).getName();
		if(spChanged->existsIngredient(strRecipeItem))
		{
			// (Previous) recipe item added
			m_RecipeItemChanges[strRecipe].m_Removed.push_back(strRecipeItem);
			bChanged = true;
			continue;
		}
		const Ingredient& rIngredientChanged = spChanged->getIngredient(strRecipeItem);
		if(!rRecipeChanged.existsRecipeItem(rIngredientChanged))
		{
			m_RecipeItemChanges[strRecipe].m_Removed.push_back(strRecipeItem);
			bChanged = true;
		}
	}

	return bChanged;
}

void RecipeBookChanges::compareRBConfigurations(const QSharedPointer<RecipeBook> spBase, const QSharedPointer<RecipeBook> spChanged)
{
	// Verify changed and added recipe book configurations
	for(quint32 i = 0; i < spChanged->getConfigurationsCount(); ++i)
	{
		QString strConfig = spChanged->getConfigurationAt(i).getName();
		if(!spBase->existsConfiguration(strConfig))
		{
			// Recipe book configurations added
			m_RBConfigurationsChanges.m_Added.push_back(strConfig);
		}
		else
		{
			// Recipe book configurations changed?
			const RecipeBookConfiguration& rChanged = spChanged->getConfigurationAt(i);
			const RecipeBookConfiguration& rBase = spBase->getConfiguration(strConfig);
			bool bChanged = compareRBConfigItems(strConfig, rBase, rChanged, spBase, spChanged);
			if(bChanged || rChanged != rBase)
			{
				m_RBConfigurationsChanges.m_Changed.append(strConfig);
			}
		}
	}

	// Find removed recipe book configurations
	for(quint32 i = 0; i < spBase->getConfigurationsCount(); ++i)
	{
		QString strConfig = spBase->getConfigurationAt(i).getName();
		if(!spChanged->existsConfiguration(strConfig))
		{
			m_RBConfigurationsChanges.m_Removed.push_back(strConfig);
		}
	}
}

bool RecipeBookChanges::compareRBConfigItems(QString strConfiguration,
											 const RecipeBookConfiguration& rConfigBase,
											 const RecipeBookConfiguration& rConfigChanged,
											 const QSharedPointer<RecipeBook> spBase,
											 const QSharedPointer<RecipeBook> spChanged)
{
	bool bChanged = false;

	// Verify changed and added recipe book configuration items
	for(quint32 i = 0; i < rConfigChanged.getItemsCount(); ++i)
	{
		QString strConfigItem = rConfigChanged.getItemAt(i).getName();
		if(!spBase->existsRecipe(strConfigItem))
		{
			// (New) recipe book configuration items
			m_RBConfigItemChanges[strConfiguration].m_Added.push_back(strConfigItem);
			bChanged = true;
			continue;
		}
		const Recipe& rRecipeBase = spBase->getRecipe(strConfigItem);
		if(!rConfigBase.existsRecipe(rRecipeBase))
		{
			// Recipe book configuration items added
			m_RBConfigItemChanges[strConfiguration].m_Added.push_back(strConfigItem);
			bChanged = true;
		}
		else
		{
			if(i >= rConfigBase.getItemsCount())
			{
				m_RBConfigItemChanges[strConfiguration].m_Changed.push_back(strConfigItem);
				bChanged = true;
				continue;
			}
			// Recipe book configuration items changed?
			const RecipeBookConfigItem& rItemChanged = rConfigChanged.getItemAt(i);
			const RecipeBookConfigItem& rItemBase = rConfigBase.getItemAt(i);
			if(rItemChanged != rItemBase)
			{
				m_RBConfigItemChanges[strConfiguration].m_Changed.push_back(strConfigItem);
				bChanged = true;
			}
		}
	}

	// Find removed recipe book configuration items
	for(quint32 i = 0; i < rConfigBase.getItemsCount(); ++i)
	{
		QString strConfigItem = rConfigBase.getItemAt(i).getName();
		if(spChanged->existsRecipe(strConfigItem))
		{
			// (Previous) recipe item added
			m_RecipeItemChanges[strConfiguration].m_Removed.push_back(strConfigItem);
			bChanged = true;
			continue;
		}
		const Recipe& rRecipeChanged = spChanged->getRecipe(strConfigItem);
		if(!rConfigChanged.existsRecipe(rRecipeChanged))
		{
			m_RBConfigItemChanges[strConfiguration].m_Removed.push_back(strConfigItem);
			bChanged = true;
		}
	}

	return bChanged;
}

const RecipeBookChanges::ItemChanges RecipeBookChanges::getRecipeChanges(QString strRecipe) const
{
	if(!m_RecipeItemChanges.contains(strRecipe))
	{
		return ItemChanges();
	}

	return m_RecipeItemChanges[strRecipe];
}

const RecipeBookChanges::ItemChanges RecipeBookChanges::getRBConfigItemChanges(QString strConfiguration) const
{
	if(!m_RBConfigItemChanges.contains(strConfiguration))
	{
		return ItemChanges();
	}

	return m_RBConfigItemChanges[strConfiguration];
}
