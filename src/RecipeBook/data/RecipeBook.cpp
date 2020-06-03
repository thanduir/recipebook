#include "RecipeBook.h"
#include <QException>
#include "Category.h"
#include "Ingredient.h"
#include "AlternativesType.h"
#include "Recipe.h"
#include "RecipeItem.h"
#include "ShoppingRecipe.h"
#include "SortOrder.h"
#include "RecipeBookConfiguration.h"
#include "util/ItemsUtil.h"

using namespace recipebook;

RecipeBook::RecipeBook()
{
}

void RecipeBook::clearData()
{
	m_Categories.clear();
	m_SortOrders.clear();
	m_Ingredients.clear();
	m_AlternativesTypes.clear();
	m_Recipes.clear();
	m_ShoppingRecipes.clear();
	m_Configurations.clear();
}

Category& RecipeBook::addCategory(QString strName)
{
	Category& rCategory = internal::sorted::addItem<Category>(Category::getElementId(strName), m_Categories, [strName, this]()
	{
		return new Category(strName);
	});

	// Also add to SortOrders
	for(QSharedPointer<SortOrder> spSortOrder : qAsConst(m_SortOrders))
	{
		spSortOrder->m_Categories.append(&rCategory);
	}

	return rCategory;
}

void RecipeBook::renameCategory(Category& rCategory, QString strNewName)
{
	internal::sorted::moveForNewIdString(rCategory, Category::getElementId(strNewName), m_Categories);
	rCategory.rename(strNewName);
}

bool RecipeBook::existsCategory(QString strName) const
{
	return internal::sorted::exists<Category>(Category::getElementId(strName), m_Categories);
}

bool RecipeBook::isCategoryInUse(const Category& rCategory, QList<Ingredient*>* pIngredients) const
{
	if(pIngredients)
	{
		pIngredients->clear();
	}

	for(QSharedPointer<Ingredient> ingredient : qAsConst(m_Ingredients))
	{
		if(helper::compare(ingredient->getCategory().getElementId(), rCategory.getElementId()) == 0)
		{
			if(pIngredients)
			{
				pIngredients->append(ingredient.get());
			}
			else
			{
				return true;
			}
		}
	}

	if(pIngredients)
	{
		return pIngredients->size() > 0;
	}

	return false;
}

bool RecipeBook::removeCategory(const Category& rCategory)
{
	if(isCategoryInUse(rCategory))
	{
		return false;
	}

	// Remove Category from all SortOrders first
	for(QSharedPointer<SortOrder> spSortOrder : qAsConst(m_SortOrders))
	{
		for(int i = 0; i < spSortOrder->m_Categories.size(); ++i)
		{
			if(helper::compare(spSortOrder->m_Categories[i]->getElementId(), rCategory.getElementId()) == 0)
			{
				spSortOrder->m_Categories.removeAt(i);
				break;
			}
		}
	}

	internal::sorted::remove(rCategory.getElementId(), m_Categories);

	return true;
}

Category& RecipeBook::getCategory(QString strName)
{
	return internal::sorted::getItem(Category::getElementId(strName), m_Categories);
}

const Category& RecipeBook::getCategory(QString strName) const
{
	return internal::sorted::getItem(Category::getElementId(strName), m_Categories);
}

const Category& RecipeBook::getDefaultCategory() const
{
	if(m_Categories.size() > 0)
	{
		return *m_Categories.front().get();
	}
	throw QException();
}

quint32 RecipeBook::getCategoriesCount() const 
{ 
	return m_Categories.size(); 
}

Category& RecipeBook::getCategoryAt(quint32 i)
{
	if(i >= (quint32)m_Categories.size())
	{
		throw QException();
	}
	return *m_Categories.at(i).get();
}

const Category& RecipeBook::getCategoryAt(quint32 i) const
{
	if(i >= (quint32)m_Categories.size())
	{
		throw QException();
	}
	return *m_Categories.at(i).get();
}

quint32 RecipeBook::getCategoryIndex(QString strName) const
{
	auto iter = recipebook::internal::helper::findItemSorted(Category::getElementId(strName), m_Categories);
	return iter - m_Categories.begin();
}

SortOrder& RecipeBook::addSortOrder(QString strName)
{
	return internal::sorted::addItem<SortOrder>(SortOrder::getElementId(strName), m_SortOrders, [strName, this]()
	{
		return new SortOrder(strName, m_Categories);
	});
}

void RecipeBook::renameSortOrder(SortOrder& rOrder, QString strNewName)
{
	internal::sorted::moveForNewIdString(rOrder, SortOrder::getElementId(strNewName), m_SortOrders);
	rOrder.rename(strNewName);
}

bool RecipeBook::existsSortOrder(QString strName) const
{
	return internal::sorted::exists<SortOrder>(SortOrder::getElementId(strName), m_SortOrders);
}

bool RecipeBook::removeSortOrder(const SortOrder& rOrder)
{
	// Remove sort order from disallowed provenances first
	for(QSharedPointer<Ingredient> ingredient : qAsConst(m_Ingredients))
	{
		if(!ingredient->provenanceAvailable(rOrder))
		{
			ingredient->setProvenanceAvailable(rOrder);
		}
	}

	internal::sorted::remove(rOrder.getElementId(), m_SortOrders);
	return true;
}

SortOrder& RecipeBook::getSortOrder(QString strName)
{
	return internal::sorted::getItem(SortOrder::getElementId(strName), m_SortOrders);
}

const SortOrder& RecipeBook::getSortOrder(QString strName) const
{
	return internal::sorted::getItem(SortOrder::getElementId(strName), m_SortOrders);
}

quint32 RecipeBook::getSortOrdersCount() const 
{ 
	return m_SortOrders.size(); 
}

SortOrder& RecipeBook::getSortOrderAt(quint32 i)
{
	if(i >= (quint32)m_SortOrders.size())
	{
		throw QException();
	}
	return *m_SortOrders.at(i).get();
}

const SortOrder& RecipeBook::getSortOrderAt(quint32 i) const
{
	if(i >= (quint32)m_SortOrders.size())
	{
		throw QException();
	}
	return *m_SortOrders.at(i).get();
}

quint32 RecipeBook::getSortOrderIndex(QString strName) const
{
	auto iter = recipebook::internal::helper::findItemSorted(SortOrder::getElementId(strName), m_SortOrders);
	return iter - m_SortOrders.begin();
}

Ingredient& RecipeBook::addIngredient(QString strName, const Category& rCategory, Unit defaultUnit)
{
	return internal::sorted::addItem<Ingredient>(Ingredient::getElementId(strName), m_Ingredients, [strName, &rCategory, defaultUnit]()
	{
		return new Ingredient(strName, rCategory, defaultUnit);
	});
}

void RecipeBook::renameIngredient(Ingredient& rIngredient, QString strNewName)
{
	internal::sorted::moveForNewIdString(rIngredient, Ingredient::getElementId(strNewName), m_Ingredients);
	rIngredient.rename(strNewName);

	// Inform shopping recipes
	for(QSharedPointer<ShoppingRecipe> recipe : qAsConst(m_ShoppingRecipes))
	{
		recipe->onIngredientNameChanged(rIngredient);
	}
}

bool RecipeBook::existsIngredient(QString strName) const
{
	return internal::sorted::exists<Ingredient>(Ingredient::getElementId(strName), m_Ingredients);
}

bool RecipeBook::isIngredientInUse(const Ingredient& rIngredient, QList<Recipe*>* pRecipes, QList<ShoppingRecipe*>* pShoppingRecipes) const
{
	if(pRecipes && pShoppingRecipes)
	{
		pRecipes->clear();
	}

	for(QSharedPointer<Recipe> spRecipe : qAsConst(m_Recipes))
	{
		if(spRecipe->existsRecipeItem(rIngredient))
		{
			if(pRecipes && pShoppingRecipes)
			{
				pRecipes->append(spRecipe.get());
				continue;
			}

			return true;
		}
	}

	if(pShoppingRecipes)
	{
		pShoppingRecipes->clear();
	}

	for(QSharedPointer<ShoppingRecipe> spShoppingRecipe : qAsConst(m_ShoppingRecipes))
	{
		if(spShoppingRecipe->existsItem(rIngredient))
		{
			if(pRecipes && pShoppingRecipes)
			{
				pShoppingRecipes->append(spShoppingRecipe.get());
			}
			else
			{
				return true;
			}
		}
	}

	if(pRecipes && pShoppingRecipes)
	{
		return pRecipes->size() > 0 || pShoppingRecipes->size() > 0;
	}

	return false;
}

bool RecipeBook::removeIngredient(const Ingredient& rIngredient)
{
	if(isIngredientInUse(rIngredient))
	{
		return false;
	}

	internal::sorted::remove(rIngredient.getElementId(), m_Ingredients);
	return true;
}

Ingredient& RecipeBook::getIngredient(QString strName)
{
	return internal::sorted::getItem(Ingredient::getElementId(strName), m_Ingredients);
}

const Ingredient& RecipeBook::getIngredient(QString strName) const
{
	return internal::sorted::getItem(Ingredient::getElementId(strName), m_Ingredients);
}

quint32 RecipeBook::getIngredientsCount() const 
{ 
	return m_Ingredients.size(); 
}

Ingredient& RecipeBook::getIngredientAt(quint32 i)
{
	if(i >= (quint32)m_Ingredients.size())
	{
		throw QException();
	}
	return *m_Ingredients.at(i).get();
}

const Ingredient& RecipeBook::getIngredientAt(quint32 i) const
{
	if(i >= (quint32)m_Ingredients.size())
	{
		throw QException();
	}
	return *m_Ingredients.at(i).get();
}

quint32 RecipeBook::getIngredientIndex(QString strName) const
{
	auto iter = recipebook::internal::helper::findItemSorted(Ingredient::getElementId(strName), m_Ingredients);
	return iter - m_Ingredients.begin();
}

AlternativesType& RecipeBook::addAlternativesType(QString strName)
{
	return internal::sorted::addItem<AlternativesType>(AlternativesType::getElementId(strName), m_AlternativesTypes, [strName, this]()
	{
		return new AlternativesType(strName);
	});
}

void RecipeBook::renameAlternativesType(AlternativesType& rType, QString strNewName)
{
	internal::sorted::moveForNewIdString(rType, AlternativesType::getElementId(strNewName), m_AlternativesTypes);
	rType.rename(strNewName);

	// Inform shopping recipes
	for(QSharedPointer<ShoppingRecipe> recipe : qAsConst(m_ShoppingRecipes))
	{
		recipe->onAlternativesTypeNameChanged(rType);
	}
}

bool RecipeBook::existsAlternativesType(QString strName) const
{
	return internal::sorted::exists<AlternativesType>(AlternativesType::getElementId(strName), m_AlternativesTypes);
}

bool RecipeBook::isAlternativesTypeInUse(const AlternativesType& rType, QList<Recipe*>* pRecipes) const
{
	if(pRecipes)
	{
		pRecipes->clear();
	}

	for(QSharedPointer<Recipe> recipe : qAsConst(m_Recipes))
	{
		for(quint32 i = 0; i < recipe->getRecipeItemsCount(); ++i)
		{
			const RecipeItem& rItem = recipe->getRecipeItemAt(i);
			if(rItem.hasAlternativesGroup() && helper::compare(rItem.getAlternativesGroup().getElementId(), rType.getElementId()) == 0)
			{
				if(pRecipes)
				{
					pRecipes->append(recipe.get());
					continue;
				}
				else
				{
					return true;
				}
			}
		}
	}

	if(pRecipes)
	{
		return pRecipes->size() > 0;
	}

	return false;
}

bool RecipeBook::removeAlternativesType(const AlternativesType& rType)
{
	if(isAlternativesTypeInUse(rType))
	{
		return false;
	}

	internal::sorted::remove(rType.getElementId(), m_AlternativesTypes);

	return true;
}

AlternativesType& RecipeBook::getAlternativesType(QString strName)
{
	return internal::sorted::getItem(AlternativesType::getElementId(strName), m_AlternativesTypes);
}

const AlternativesType& RecipeBook::getAlternativesType(QString strName) const
{
	return internal::sorted::getItem(AlternativesType::getElementId(strName), m_AlternativesTypes);
}

quint32 RecipeBook::getAlternativesTypesCount() const
{
	return m_AlternativesTypes.count();
}

AlternativesType& RecipeBook::getAlternativesTypeAt(quint32 i)
{
	if(i >= (quint32)m_AlternativesTypes.size())
	{
		throw QException();
	}
	return *m_AlternativesTypes.at(i).get();
}

const AlternativesType& RecipeBook::getAlternativesTypeAt(quint32 i) const
{
	if(i >= (quint32)m_AlternativesTypes.size())
	{
		throw QException();
	}
	return *m_AlternativesTypes.at(i).get();
}

quint32 RecipeBook::getAlternativesTypeIndex(QString strName) const
{
	auto iter = recipebook::internal::helper::findItemSorted(AlternativesType::getElementId(strName), m_AlternativesTypes);
	return iter - m_AlternativesTypes.begin();
}

Recipe& RecipeBook::addRecipe(QString strName, quint32 uiNrPersons)
{
	return internal::sorted::addItem<Recipe>(Recipe::getElementId(strName), m_Recipes, [strName, uiNrPersons]()
	{
		return new Recipe(strName, uiNrPersons);
	});
}

void RecipeBook::renameRecipe(Recipe& rRecipe, QString strNewName)
{
	internal::sorted::moveForNewIdString(rRecipe, Recipe::getElementId(strNewName), m_Recipes);
	rRecipe.rename(strNewName);
}

bool RecipeBook::existsRecipe(QString strName) const
{
	return internal::sorted::exists<Recipe>(Recipe::getElementId(strName), m_Recipes);
}

bool RecipeBook::removeRecipe(const Recipe& rRecipe)
{
	// Remove recipe from all configurations
	for(QSharedPointer<RecipeBookConfiguration> config : qAsConst(m_Configurations))
	{
		config->removeRecipe(rRecipe);
	}

	internal::sorted::remove(rRecipe.getElementId(), m_Recipes);
	return true;
}

const Recipe& RecipeBook::copyRecipe(const Recipe& rRecipe, QString strNewName)
{
	return internal::sorted::addItem<Recipe>(Recipe::getElementId(strNewName), m_Recipes, [strNewName, &rRecipe]()
	{
		return new Recipe(strNewName, rRecipe);
	});
}

Recipe& RecipeBook::getRecipe(QString strName)
{
	return internal::sorted::getItem(Recipe::getElementId(strName), m_Recipes);
}

const Recipe& RecipeBook::getRecipe(QString strName) const
{
	return internal::sorted::getItem(Recipe::getElementId(strName), m_Recipes);
}

quint32 RecipeBook::getRecipesCount() const 
{ 
	return m_Recipes.size(); 
}

Recipe& RecipeBook::getRecipeAt(quint32 i)
{
	if(i >= (quint32)m_Recipes.size())
	{
		throw QException();
	}
	return *m_Recipes.at(i).get();
}

const Recipe& RecipeBook::getRecipeAt(quint32 i) const
{
	if(i >= (quint32)m_Recipes.size())
	{
		throw QException();
	}
	return *m_Recipes.at(i).get();
}

quint32 RecipeBook::getRecipeIndex(QString strName) const
{
	auto iter = recipebook::internal::helper::findItemSorted(Recipe::getElementId(strName), m_Recipes);
	return iter - m_Recipes.begin();
}

ShoppingRecipe& RecipeBook::addNewShoppingRecipe(QString strName, float fScalingFactor)
{
	return internal::sorted::addItem<ShoppingRecipe>(ShoppingRecipe::getElementId(strName), m_ShoppingRecipes, [strName, fScalingFactor]()
	{
		return new ShoppingRecipe(strName, fScalingFactor);
	});
}

ShoppingRecipe& RecipeBook::addShoppingRecipe(QString strName, const Recipe& rRecipe)
{
	return internal::sorted::addItem<ShoppingRecipe>(ShoppingRecipe::getElementId(strName), m_ShoppingRecipes, [strName, &rRecipe]()
	{
		return new ShoppingRecipe(strName, rRecipe);
	});
}

void RecipeBook::renameShoppingRecipe(ShoppingRecipe& rRecipe, QString strNewName)
{
	internal::sorted::moveForNewIdString(rRecipe, ShoppingRecipe::getElementId(strNewName), m_ShoppingRecipes);
	rRecipe.rename(strNewName);
}

bool RecipeBook::existsShoppingRecipe(QString strName) const
{
	return internal::sorted::exists<ShoppingRecipe>(ShoppingRecipe::getElementId(strName), m_ShoppingRecipes);
}

bool RecipeBook::removeShoppingRecipe(const ShoppingRecipe& rRecipe)
{
	internal::sorted::remove(rRecipe.getElementId(), m_ShoppingRecipes);
	return true;
}

ShoppingRecipe& RecipeBook::getShoppingRecipe(QString strName)
{
	return internal::sorted::getItem(ShoppingRecipe::getElementId(strName), m_ShoppingRecipes);
}

const ShoppingRecipe& RecipeBook::getShoppingRecipe(QString strName) const
{
	return internal::sorted::getItem(ShoppingRecipe::getElementId(strName), m_ShoppingRecipes);
}

quint32 RecipeBook::getShoppingRecipesCount() const 
{ 
	return m_ShoppingRecipes.size(); 
}

ShoppingRecipe& RecipeBook::getShoppingRecipeAt(quint32 i)
{
	if(i >= (quint32)m_ShoppingRecipes.size())
	{
		throw QException();
	}
	return *m_ShoppingRecipes.at(i).get();
}

const ShoppingRecipe& RecipeBook::getShoppingRecipeAt(quint32 i) const
{
	if(i >= (quint32)m_ShoppingRecipes.size())
	{
		throw QException();
	}
	return *m_ShoppingRecipes.at(i).get();
}

quint32 RecipeBook::getShoppingRecipeIndex(QString strName) const
{
	auto iter = recipebook::internal::helper::findItemSorted(ShoppingRecipe::getElementId(strName), m_ShoppingRecipes);
	return iter - m_ShoppingRecipes.begin();
}

void RecipeBook::clearShoppingList()
{
	m_ShoppingRecipes.clear();
}

RecipeBookConfiguration& RecipeBook::addConfiguration(QString strName, bool bAddAllRecipes)
{
	return internal::sorted::addItem<RecipeBookConfiguration>(RecipeBookConfiguration::getElementId(strName), m_Configurations, [this, strName, bAddAllRecipes]()
	{
		return new RecipeBookConfiguration(strName, bAddAllRecipes ? this : nullptr);
	});
}

void RecipeBook::renameConfiguration(RecipeBookConfiguration& rConfig, QString strNewName)
{
	internal::sorted::moveForNewIdString(rConfig, RecipeBookConfiguration::getElementId(strNewName), m_Configurations);
	rConfig.rename(strNewName);
}

bool RecipeBook::existsConfiguration(QString strName) const
{
	return internal::sorted::exists<RecipeBookConfiguration>(RecipeBookConfiguration::getElementId(strName), m_Configurations);
}

bool RecipeBook::removeConfiguration(const RecipeBookConfiguration& rConfig)
{
	internal::sorted::remove(rConfig.getElementId(), m_Configurations);
	return true;
}

const RecipeBookConfiguration& RecipeBook::copyConfiguration(const RecipeBookConfiguration& rConfig, QString strNewName)
{
	return internal::sorted::addItem<RecipeBookConfiguration>(RecipeBookConfiguration::getElementId(strNewName), m_Configurations, [strNewName, &rConfig]()
	{
		return new RecipeBookConfiguration(strNewName, rConfig);
	});
}

RecipeBookConfiguration& RecipeBook::getConfiguration(QString strName)
{
	return internal::sorted::getItem(RecipeBookConfiguration::getElementId(strName), m_Configurations);
}

const RecipeBookConfiguration& RecipeBook::getConfiguration(QString strName) const
{
	return internal::sorted::getItem(RecipeBookConfiguration::getElementId(strName), m_Configurations);
}

quint32 RecipeBook::getConfigurationsCount() const
{
	return m_Configurations.size();
}

RecipeBookConfiguration& RecipeBook::getConfigurationAt(quint32 i)
{
	if(i >= (quint32)m_Configurations.size())
	{
		throw QException();
	}
	return *m_Configurations.at(i).get();
}

const RecipeBookConfiguration& RecipeBook::getConfigurationAt(quint32 i) const
{
	if(i >= (quint32)m_Configurations.size())
	{
		throw QException();
	}
	return *m_Configurations.at(i).get();
}

quint32 RecipeBook::getConfigurationIndex(QString strName) const
{
	auto iter = recipebook::internal::helper::findItemSorted(RecipeBookConfiguration::getElementId(strName), m_Configurations);
	return iter - m_Configurations.begin();
}
