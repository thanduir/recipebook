#include "RecipeBook.h"
#include <QException>
#include "Category.h"
#include "Ingredient.h"
#include "Recipe.h"
#include "ShoppingRecipe.h"
#include "SortOrder.h"

using namespace recipebook;

namespace
{
    template<class T> bool exists(const QString strName, QVector<QSharedPointer<T>> allItems)
    {
        for(QSharedPointer<T> spItem : qAsConst(allItems))
        {
            if(spItem->getName() == strName)
            {
                return true;
            }
        }
        return false;
    }

    template<class T> void remove(const T& rItem, QVector<QSharedPointer<T>>& allItems)
    {
        for(int i = 0; i < allItems.size(); ++i)
        {
            if(allItems[i]->getName() == rItem.getName())
            {
                allItems.remove(i);
                return;
            }
        }
    }

    template<class T> QStringList getAllNames(QVector<QSharedPointer<T>> allItems, bool bSorted = true)
    {
        QStringList list;
        for(QSharedPointer<T> spItem : qAsConst(allItems))
        {
            list.append(spItem->getName());
        }
        if(bSorted)
        {
            // TODO: Collections.sort(vec, new Helper.SortStringIgnoreCase());
        }
        return list;
    }

    template<class T> const T& getItemConst(QString strName, const QVector<QSharedPointer<T>>& allItems)
    {
        for(QSharedPointer<T> spItem : qAsConst(allItems))
        {
            if(spItem->getName() == strName)
            {
                return *spItem.get();
            }
        }

        throw new QException();
    }

    template<class T> T& getItem(QString strName, QVector<QSharedPointer<T>> allItems)
    {
        for(QSharedPointer<T> spItem : qAsConst(allItems))
        {
            if(spItem->getName() == strName)
            {
                return *spItem.get();
            }
        }

        throw new QException();
    }
    
    template<class T, typename Func> 
    const T& addItem(QString strName, QVector<QSharedPointer<T>>& allItems, Func constructNewItem)
    {
        for(QSharedPointer<T> spItem : qAsConst(allItems))
        {
            if(spItem->getName() == strName)
            {
                return *spItem.get();
            }
        }

        QSharedPointer<T> spNewItem(constructNewItem());
        allItems.append(spNewItem);

        return *spNewItem.get();
    }
}

RecipeBook::RecipeBook()
{
}

const Category& RecipeBook::addCategory(QString strName)
{
    // TODO: Can i bring this together with my add-template-method above?
    for(QSharedPointer<Category> spCategory : qAsConst(m_Categories))
    {
        if(spCategory->getName() == strName)
        {
            return *spCategory.get();
        }
    }

    QSharedPointer<Category> category(new Category(strName));
    m_Categories.append(category);

    for(QSharedPointer<SortOrder> spSortOrder : qAsConst(m_SortOrders))
    {
        spSortOrder->m_Categories.append(category.get());
    }

    return *category.get();
}

bool RecipeBook::existsCategory(QString strName) const
{
    return exists<Category>(strName, m_Categories);
}

void RecipeBook::removeCategory(const Category& rCategory)
{
    // TODO: TEST WHETHER IT IS STILL IN USE! (ingredients!)

    remove(rCategory, m_Categories);

    // Remove Category also from all SortOrders
    for(QSharedPointer<SortOrder> spSortOrder : qAsConst(m_SortOrders))
    {
        for(int i = 0; i < spSortOrder->m_Categories.size(); ++i)
        {
            if(spSortOrder->m_Categories[i]->getName() == rCategory.getName())
            {
                spSortOrder->m_Categories.removeAt(i);
                return;
            }
        }
    }
}

const Category& RecipeBook::getCategory(QString strName) const
{
    return getItemConst(strName, m_Categories);
}

const Category& RecipeBook::getDefaultCategory() const
{
    if(m_Categories.size() > 0)
    {
        return *m_Categories.front().get();
    }
    throw new QException();
}

const SortOrder& RecipeBook::addSortOrder(QString strName)
{
    return addItem(strName, m_SortOrders, [strName, this]()
    {
        return new SortOrder(strName, m_Categories);
    });
}

bool RecipeBook::existsSortOrder(QString strName) const
{
    return exists<SortOrder>(strName, m_SortOrders);
}

void RecipeBook::removeSortOrder(const SortOrder& rOrder)
{
    // TODO: TEST WHETHER IT IS STILL IN USE! (ingredients!)

    remove(rOrder, m_SortOrders);
}

SortOrder& RecipeBook::getSortOrder(QString strName)
{
    return getItem(strName, m_SortOrders);
}

const SortOrder& RecipeBook::getSortOrder(QString strName) const
{
    return getItemConst(strName, m_SortOrders);
}

QStringList RecipeBook::getAllSortOrderNamesSorted() const
{
    return getAllNames(m_SortOrders);
}

const Ingredient& RecipeBook::addIngredient(QString strName, const Category& rCategory, Unit defaultUnit)
{
    return addItem(strName, m_Ingredients, [strName, &rCategory, defaultUnit]()
    {
        return new Ingredient(strName, rCategory, defaultUnit);
    });
}

bool RecipeBook::existsIngredient(QString strName) const
{
    return exists<Ingredient>(strName, m_Ingredients);
}

void RecipeBook::removeIngredient(const Ingredient& rIngredient)
{
    // TODO: TEST WHETHER IT IS STILL IN USE! (recipes! shoppinglists!)

    remove(rIngredient, m_Ingredients);
}

Ingredient& RecipeBook::getIngredient(QString strName)
{
    return getItem(strName, m_Ingredients);
}

const Ingredient& RecipeBook::getIngredient(QString strName) const
{
    return getItemConst(strName, m_Ingredients);
}

QStringList RecipeBook::getAllIngredientNamesSorted() const
{
    return getAllNames(m_Ingredients);
}

const Recipe& RecipeBook::addRecipe(QString strName, quint32 iNrPersons)
{
    return addItem(strName, m_Recipes, [strName]()
    {
        return new Recipe(strName);
    });
}

bool RecipeBook::existsRecipe(QString strName) const
{
    return exists<Recipe>(strName, m_Recipes);
}

void RecipeBook::removeRecipe(const Recipe& rRecipe)
{
    remove(rRecipe, m_Recipes);
}

const Recipe& RecipeBook::copyRecipe(const Recipe& rRecipe, QString strNewName)
{
    return addItem(strNewName, m_Recipes, [&rRecipe]()
    {
        return new Recipe(rRecipe);
    });
}

Recipe& RecipeBook::getRecipe(QString strName)
{
    return getItem(strName, m_Recipes);
}

const Recipe& RecipeBook::getRecipe(QString strName) const
{
    return getItemConst(strName, m_Recipes);
}

QStringList RecipeBook::getAllRecipeNamesSorted() const
{
    return getAllNames(m_Recipes);
}

const ShoppingRecipe& RecipeBook::addNewShoppingRecipe(QString strName)
{
    return addItem(strName, m_ShoppingRecipes, [strName]()
    {
        return new ShoppingRecipe(strName);
    });
}

const ShoppingRecipe& RecipeBook::addShoppingRecipe(QString strName, const Recipe& rRecipe)
{
    return addItem(strName, m_ShoppingRecipes, [&rRecipe]()
    {
        return new ShoppingRecipe(rRecipe);
    });
}

bool RecipeBook::existsShoppingRecipe(QString strName) const
{
    return exists<ShoppingRecipe>(strName, m_ShoppingRecipes);
}

void RecipeBook::removeShoppingRecipe(const ShoppingRecipe& rRecipe)
{
    remove(rRecipe, m_ShoppingRecipes);
}

ShoppingRecipe& RecipeBook::getShoppingRecipe(QString strName)
{
    return getItem(strName, m_ShoppingRecipes);
}

const ShoppingRecipe& RecipeBook::getShoppingRecipe(QString strName) const
{
    return getItemConst(strName, m_ShoppingRecipes);
}

QStringList RecipeBook::getAllShoppingRecipeNames() const
{
    return getAllNames(m_ShoppingRecipes, false);
}

void RecipeBook::clearShoppingList()
{
    m_ShoppingRecipes.clear();
}
