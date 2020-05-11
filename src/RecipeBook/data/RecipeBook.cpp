#include "RecipeBook.h"
#include <QException>
#include "Category.h"
#include "Ingredient.h"
#include "AlternativesType.h"
#include "Recipe.h"
#include "RecipeItem.h"
#include "ShoppingRecipe.h"
#include "SortOrder.h"
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
	m_Recipes.clear();
	m_ShoppingRecipes.clear();
}

Category& RecipeBook::addCategory(QString strName)
{
	Category& rCategory = internal::sorted::addItem(strName, m_Categories, [strName, this]()
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
	internal::sorted::moveForNewIdString(rCategory, strNewName, m_Categories);
	rCategory.rename(strNewName);
}

bool RecipeBook::existsCategory(QString strName) const
{
	return internal::sorted::exists<Category>(strName, m_Categories);
}

bool RecipeBook::isCategoryInUse(const Category& rCategory, QList<Ingredient*>* pIngredients) const
{
	if(pIngredients)
	{
		pIngredients->clear();
	}

	for(QSharedPointer<Ingredient> ingredient : qAsConst(m_Ingredients))
	{
		if(ingredient->getCategory().getIdString() == rCategory.getIdString())
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
			if(spSortOrder->m_Categories[i]->getIdString() == rCategory.getIdString())
			{
				spSortOrder->m_Categories.removeAt(i);
				break;
			}
		}
	}

	internal::sorted::remove(rCategory.getIdString(), m_Categories);

	return true;
}

Category& RecipeBook::getCategory(QString strName)
{
	return internal::sorted::getItem(strName, m_Categories);
}

const Category& RecipeBook::getCategory(QString strName) const
{
	return internal::sorted::getItem(strName, m_Categories);
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
	auto iter = recipebook::internal::helper::findItemSorted(strName, m_Categories);
	return iter - m_Categories.begin();
}

SortOrder& RecipeBook::addSortOrder(QString strName)
{
	return internal::sorted::addItem(strName, m_SortOrders, [strName, this]()
	{
		return new SortOrder(strName, m_Categories);
	});
}

void RecipeBook::renameSortOrder(SortOrder& rOrder, QString strNewName)
{
	internal::sorted::moveForNewIdString(rOrder, strNewName, m_SortOrders);
	rOrder.rename(strNewName);
}

bool RecipeBook::existsSortOrder(QString strName) const
{
	return internal::sorted::exists<SortOrder>(strName, m_SortOrders);
}

bool RecipeBook::isSortOrderInUse(const SortOrder& rOrder, QList<Ingredient*>* pIngredients) const
{
	if(pIngredients)
	{
		pIngredients->clear();
	}

	for(QSharedPointer<Ingredient> ingredient : qAsConst(m_Ingredients))
	{
		if(!ingredient->hasProvenanceEverywhere()
			&& ingredient->getProvenance().getIdString() == rOrder.getIdString())
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

bool RecipeBook::removeSortOrder(const SortOrder& rOrder)
{
	if(isSortOrderInUse(rOrder))
	{
		return false;
	}

	internal::sorted::remove(rOrder.getIdString(), m_SortOrders);
	return true;
}

SortOrder& RecipeBook::getSortOrder(QString strName)
{
	return internal::sorted::getItem(strName, m_SortOrders);
}

const SortOrder& RecipeBook::getSortOrder(QString strName) const
{
	return internal::sorted::getItem(strName, m_SortOrders);
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
	auto iter = recipebook::internal::helper::findItemSorted(strName, m_SortOrders);
	return iter - m_SortOrders.begin();
}

Ingredient& RecipeBook::addIngredient(QString strName, const Category& rCategory, Unit defaultUnit)
{
	return internal::sorted::addItem(strName, m_Ingredients, [strName, &rCategory, defaultUnit]()
	{
		return new Ingredient(strName, rCategory, defaultUnit);
	});
}

void RecipeBook::renameIngredient(Ingredient& rIngredient, QString strNewName)
{
	internal::sorted::moveForNewIdString(rIngredient, strNewName, m_Ingredients);
	rIngredient.rename(strNewName);

	// Inform shopping recipes
	for(QSharedPointer<ShoppingRecipe> recipe : qAsConst(m_ShoppingRecipes))
	{
		recipe->onIngredientNameChanged(rIngredient);
	}
}

bool RecipeBook::existsIngredient(QString strName) const
{
	return internal::sorted::exists<Ingredient>(strName, m_Ingredients);
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

	internal::sorted::remove(rIngredient.getIdString(), m_Ingredients);
	return true;
}

Ingredient& RecipeBook::getIngredient(QString strName)
{
	return internal::sorted::getItem(strName, m_Ingredients);
}

const Ingredient& RecipeBook::getIngredient(QString strName) const
{
	return internal::sorted::getItem(strName, m_Ingredients);
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
	auto iter = recipebook::internal::helper::findItemSorted(strName, m_Ingredients);
	return iter - m_Ingredients.begin();
}

AlternativesType& RecipeBook::addAlternativesType(QString strName)
{
	return internal::sorted::addItem(strName, m_AlternativesTypes, [strName, this]()
	{
		return new AlternativesType(strName);
	});
}

void RecipeBook::renameAlternativesType(AlternativesType& rType, QString strNewName)
{
	internal::sorted::moveForNewIdString(rType, strNewName, m_AlternativesTypes);
	rType.rename(strNewName);

	// Inform shopping recipes
	for(QSharedPointer<ShoppingRecipe> recipe : qAsConst(m_ShoppingRecipes))
	{
		recipe->onAlternativesTypeNameChanged(rType);
	}
}

bool RecipeBook::existsAlternativesType(QString strName) const
{
	return internal::sorted::exists<AlternativesType>(strName, m_AlternativesTypes);
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
			if(rItem.hasAlternativesGroup() && rItem.getAlternativesGroup().getIdString() == rType.getIdString())
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

	internal::sorted::remove(rType.getIdString(), m_AlternativesTypes);

	return true;
}

AlternativesType& RecipeBook::getAlternativesType(QString strName)
{
	return internal::sorted::getItem(strName, m_AlternativesTypes);
}

const AlternativesType& RecipeBook::getAlternativesType(QString strName) const
{
	return internal::sorted::getItem(strName, m_AlternativesTypes);
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
	auto iter = recipebook::internal::helper::findItemSorted(strName, m_AlternativesTypes);
	return iter - m_AlternativesTypes.begin();
}

Recipe& RecipeBook::addRecipe(QString strName, quint32 uiNrPersons)
{
	return internal::sorted::addItem(strName, m_Recipes, [strName, uiNrPersons]()
	{
		return new Recipe(strName, uiNrPersons);
	});
}

void RecipeBook::renameRecipe(Recipe& rRecipe, QString strNewName)
{
	internal::sorted::moveForNewIdString(rRecipe, strNewName, m_Recipes);
	rRecipe.rename(strNewName);
}

bool RecipeBook::existsRecipe(QString strName) const
{
	return internal::sorted::exists<Recipe>(strName, m_Recipes);
}

bool RecipeBook::removeRecipe(const Recipe& rRecipe)
{
	internal::sorted::remove(rRecipe.getIdString(), m_Recipes);
	return true;
}

const Recipe& RecipeBook::copyRecipe(const Recipe& rRecipe, QString strNewName)
{
	return internal::sorted::addItem(strNewName, m_Recipes, [strNewName, &rRecipe]()
	{
		return new Recipe(strNewName, rRecipe);
	});
}

Recipe& RecipeBook::getRecipe(QString strName)
{
	return internal::sorted::getItem(strName, m_Recipes);
}

const Recipe& RecipeBook::getRecipe(QString strName) const
{
	return internal::sorted::getItem(strName, m_Recipes);
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
	auto iter = recipebook::internal::helper::findItemSorted(strName, m_Recipes);
	return iter - m_Recipes.begin();
}

ShoppingRecipe& RecipeBook::addNewShoppingRecipe(QString strName, float fScalingFactor)
{
	return internal::sorted::addItem(strName, m_ShoppingRecipes, [strName, fScalingFactor]()
	{
		return new ShoppingRecipe(strName, fScalingFactor);
	});
}

ShoppingRecipe& RecipeBook::addShoppingRecipe(QString strName, const Recipe& rRecipe)
{
	return internal::sorted::addItem(strName, m_ShoppingRecipes, [strName, &rRecipe]()
	{
		return new ShoppingRecipe(strName, rRecipe);
	});
}

void RecipeBook::renameShoppingRecipe(ShoppingRecipe& rRecipe, QString strNewName)
{
	internal::sorted::moveForNewIdString(rRecipe, strNewName, m_ShoppingRecipes);
	rRecipe.rename(strNewName);
}

bool RecipeBook::existsShoppingRecipe(QString strName) const
{
	return internal::sorted::exists<ShoppingRecipe>(strName, m_ShoppingRecipes);
}

bool RecipeBook::removeShoppingRecipe(const ShoppingRecipe& rRecipe)
{
	internal::sorted::remove(rRecipe.getIdString(), m_ShoppingRecipes);
	return true;
}

ShoppingRecipe& RecipeBook::getShoppingRecipe(QString strName)
{
	return internal::sorted::getItem(strName, m_ShoppingRecipes);
}

const ShoppingRecipe& RecipeBook::getShoppingRecipe(QString strName) const
{
	return internal::sorted::getItem(strName, m_ShoppingRecipes);
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
	auto iter = recipebook::internal::helper::findItemSorted(strName, m_ShoppingRecipes);
	return iter - m_ShoppingRecipes.begin();
}

void RecipeBook::clearShoppingList()
{
	m_ShoppingRecipes.clear();
}
