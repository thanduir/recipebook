#include "RecipeBook.h"
#include <QException>
#include "Category.h"
#include "Ingredient.h"
#include "Recipe.h"
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
    Category& rCategory = internal::addItem(strName, m_Categories, [strName, this]()
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

bool RecipeBook::existsCategory(QString strName) const
{
    return internal::exists<Category>(strName, m_Categories);
}

bool RecipeBook::removeCategory(const Category& rCategory)
{
    // Removing is only allow if it's not in use anymore
    for(QSharedPointer<Ingredient> ingredient : qAsConst(m_Ingredients))
    {
        if(ingredient->getCategory().getName() == rCategory.getName())
        {
            return false;
        }
    }

    internal::remove(rCategory, m_Categories);

    // Remove Category also from all SortOrders
    for(QSharedPointer<SortOrder> spSortOrder : qAsConst(m_SortOrders))
    {
        for(int i = 0; i < spSortOrder->m_Categories.size(); ++i)
        {
            if(spSortOrder->m_Categories[i]->getName() == rCategory.getName())
            {
                spSortOrder->m_Categories.removeAt(i);
                break;
            }
        }
    }

    return true;
}

const Category& RecipeBook::getCategory(QString strName) const
{
    return internal::getItemConst(strName, m_Categories);
}

const Category& RecipeBook::getDefaultCategory() const
{
    if(m_Categories.size() > 0)
    {
        return *m_Categories.front().get();
    }
    throw QException();
}

QStringList RecipeBook::getAllCategoryNamesSorted() const
{
    return internal::getAllNames(m_Categories);
}

SortOrder& RecipeBook::addSortOrder(QString strName)
{
    return internal::addItem(strName, m_SortOrders, [strName, this]()
    {
        return new SortOrder(strName, m_Categories);
    });
}

bool RecipeBook::existsSortOrder(QString strName) const
{
    return internal::exists<SortOrder>(strName, m_SortOrders);
}

bool RecipeBook::removeSortOrder(const SortOrder& rOrder)
{
    // Removing is only allow if it's not in use anymore
    for(QSharedPointer<Ingredient> ingredient : qAsConst(m_Ingredients))
    {
        if(ingredient->getProvenance().getName() == rOrder.getName())
        {
            return false;
        }
    }

    internal::remove(rOrder, m_SortOrders);
    return true;
}

SortOrder& RecipeBook::getSortOrder(QString strName)
{
    return internal::getItem(strName, m_SortOrders);
}

const SortOrder& RecipeBook::getSortOrder(QString strName) const
{
    return internal::getItemConst(strName, m_SortOrders);
}

QStringList RecipeBook::getAllSortOrderNamesSorted() const
{
    return internal::getAllNames(m_SortOrders);
}

Ingredient& RecipeBook::addIngredient(QString strName, const Category& rCategory, Unit defaultUnit)
{
    return internal::addItem(strName, m_Ingredients, [strName, &rCategory, defaultUnit]()
    {
        return new Ingredient(strName, rCategory, defaultUnit);
    });
}

bool RecipeBook::existsIngredient(QString strName) const
{
    return internal::exists<Ingredient>(strName, m_Ingredients);
}

bool RecipeBook::removeIngredient(const Ingredient& rIngredient)
{
    // Removing is only allow if it's not in use anymore
    for(QSharedPointer<Recipe> spRecipe : qAsConst(m_Recipes))
    {
        if(spRecipe->getRecipeItems().existsItem(rIngredient))
        {
            return false;
        }

        for(QSharedPointer<RecipeItemGroup> spGroup : qAsConst(spRecipe->m_ItemGroups))
        {
            if(spGroup->existsItem(rIngredient))
            {
                return false;
            }
        }
    }
    for(QSharedPointer<ShoppingRecipe> spShoppingRecipe : qAsConst(m_ShoppingRecipes))
    {
        if(spShoppingRecipe->existsItem(rIngredient))
        {
            return false;
        }
    }

    internal::remove(rIngredient, m_Ingredients);
    return true;
}

Ingredient& RecipeBook::getIngredient(QString strName)
{
    return internal::getItem(strName, m_Ingredients);
}

const Ingredient& RecipeBook::getIngredient(QString strName) const
{
    return internal::getItemConst(strName, m_Ingredients);
}

QStringList RecipeBook::getAllIngredientNamesSorted() const
{
    return internal::getAllNames(m_Ingredients);
}

Recipe& RecipeBook::addRecipe(QString strName, quint32 uiNrPersons)
{
    return internal::addItem(strName, m_Recipes, [strName, uiNrPersons]()
    {
        return new Recipe(strName, uiNrPersons);
    });
}

bool RecipeBook::existsRecipe(QString strName) const
{
    return internal::exists<Recipe>(strName, m_Recipes);
}

bool RecipeBook::removeRecipe(const Recipe& rRecipe)
{
    internal::remove(rRecipe, m_Recipes);
    return true;
}

const Recipe& RecipeBook::copyRecipe(const Recipe& rRecipe, QString strNewName)
{
    return internal::addItem(strNewName, m_Recipes, [strNewName, &rRecipe]()
    {
        return new Recipe(strNewName, rRecipe);
    });
}

Recipe& RecipeBook::getRecipe(QString strName)
{
    return internal::getItem(strName, m_Recipes);
}

const Recipe& RecipeBook::getRecipe(QString strName) const
{
    return internal::getItemConst(strName, m_Recipes);
}

QStringList RecipeBook::getAllRecipeNamesSorted() const
{
    return internal::getAllNames(m_Recipes);
}

ShoppingRecipe& RecipeBook::addNewShoppingRecipe(QString strName, float fScalingFactor)
{
    return internal::addItem(strName, m_ShoppingRecipes, [strName, fScalingFactor]()
    {
        return new ShoppingRecipe(strName, fScalingFactor);
    });
}

ShoppingRecipe& RecipeBook::addShoppingRecipe(QString strName, const Recipe& rRecipe)
{
    return internal::addItem(strName, m_ShoppingRecipes, [&rRecipe]()
    {
        return new ShoppingRecipe(rRecipe);
    });
}

bool RecipeBook::existsShoppingRecipe(QString strName) const
{
    return internal::exists<ShoppingRecipe>(strName, m_ShoppingRecipes);
}

bool RecipeBook::removeShoppingRecipe(const ShoppingRecipe& rRecipe)
{
    internal::remove(rRecipe, m_ShoppingRecipes);
    return true;
}

ShoppingRecipe& RecipeBook::getShoppingRecipe(QString strName)
{
    return internal::getItem(strName, m_ShoppingRecipes);
}

const ShoppingRecipe& RecipeBook::getShoppingRecipe(QString strName) const
{
    return internal::getItemConst(strName, m_ShoppingRecipes);
}

QStringList RecipeBook::getAllShoppingRecipeNames() const
{
    return internal::getAllNames(m_ShoppingRecipes, false);
}

void RecipeBook::clearShoppingList()
{
    m_ShoppingRecipes.clear();
}
