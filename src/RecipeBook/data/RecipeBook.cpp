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
    internal::sorted::moveForNewName(rCategory, strNewName, m_Categories);
    rCategory.rename(strNewName);
}

bool RecipeBook::existsCategory(QString strName) const
{
    return internal::sorted::exists<Category>(strName, m_Categories);
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

    internal::sorted::remove(rCategory, m_Categories);

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

SortOrder& RecipeBook::addSortOrder(QString strName)
{
    return internal::sorted::addItem(strName, m_SortOrders, [strName, this]()
    {
        return new SortOrder(strName, m_Categories);
    });
}

void RecipeBook::renameSortOrder(SortOrder& rOrder, QString strNewName)
{
    internal::sorted::moveForNewName(rOrder, strNewName, m_SortOrders);
    rOrder.rename(strNewName);
}

bool RecipeBook::existsSortOrder(QString strName) const
{
    return internal::sorted::exists<SortOrder>(strName, m_SortOrders);
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

    internal::sorted::remove(rOrder, m_SortOrders);
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

Ingredient& RecipeBook::addIngredient(QString strName, const Category& rCategory, Unit defaultUnit)
{
    return internal::sorted::addItem(strName, m_Ingredients, [strName, &rCategory, defaultUnit]()
    {
        return new Ingredient(strName, rCategory, defaultUnit);
    });
}

void RecipeBook::renameIngredient(Ingredient& rIngredient, QString strNewName)
{
    internal::sorted::moveForNewName(rIngredient, strNewName, m_Ingredients);
    rIngredient.rename(strNewName);
}

bool RecipeBook::existsIngredient(QString strName) const
{
    return internal::sorted::exists<Ingredient>(strName, m_Ingredients);
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

    internal::sorted::remove(rIngredient, m_Ingredients);
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

Recipe& RecipeBook::addRecipe(QString strName, quint32 uiNrPersons)
{
    return internal::sorted::addItem(strName, m_Recipes, [strName, uiNrPersons]()
    {
        return new Recipe(strName, uiNrPersons);
    });
}

void RecipeBook::renameRecipe(Recipe& rRecipe, QString strNewName)
{
    internal::sorted::moveForNewName(rRecipe, strNewName, m_Recipes);
    rRecipe.rename(strNewName);
}

bool RecipeBook::existsRecipe(QString strName) const
{
    return internal::sorted::exists<Recipe>(strName, m_Recipes);
}

bool RecipeBook::removeRecipe(const Recipe& rRecipe)
{
    internal::sorted::remove(rRecipe, m_Recipes);
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

ShoppingRecipe& RecipeBook::addNewShoppingRecipe(QString strName, float fScalingFactor)
{
    return internal::sorted::addItem(strName, m_ShoppingRecipes, [strName, fScalingFactor]()
    {
        return new ShoppingRecipe(strName, fScalingFactor);
    });
}

ShoppingRecipe& RecipeBook::addShoppingRecipe(QString strName, const Recipe& rRecipe)
{
    return internal::sorted::addItem(strName, m_ShoppingRecipes, [&rRecipe]()
    {
        return new ShoppingRecipe(rRecipe);
    });
}

void RecipeBook::renameShoppingRecipe(ShoppingRecipe& rRecipe, QString strNewName)
{
    internal::sorted::moveForNewName(rRecipe, strNewName, m_ShoppingRecipes);
    rRecipe.rename(strNewName);
}

bool RecipeBook::existsShoppingRecipe(QString strName) const
{
    return internal::sorted::exists<ShoppingRecipe>(strName, m_ShoppingRecipes);
}

bool RecipeBook::removeShoppingRecipe(const ShoppingRecipe& rRecipe)
{
    internal::sorted::remove(rRecipe, m_ShoppingRecipes);
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

void RecipeBook::clearShoppingList()
{
    m_ShoppingRecipes.clear();
}