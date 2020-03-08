#ifndef RECIPEBOOK_RECIPEBOOK_H
#define RECIPEBOOK_RECIPEBOOK_H

#include <QLinkedList>
#include <QMap>
#include <QSharedPointer>
#include <QString>
#include "Unit.h"

namespace recipebook
{
    class Category;
    class SortOrder;
    class Ingredient;
    class Recipe;
    class ShoppingRecipe;

    class RecipeBook
    {
    public:
        RecipeBook();

        // Categories

        const Category& addCategory(QString strName);
        bool existsCategory(QString strName) const;
        void removeCategory(const Category& rCategory);        
        const Category& getCategory(QString strName) const;
        const Category& getDefaultCategory() const;
                
        // SortOrders

        const SortOrder& addSortOrder(QString strName);
        bool existsSortOrder(QString strName) const;
        void removeSortOrder(const SortOrder& rOrder);
        SortOrder& getSortOrder(QString strName);
        const SortOrder& getSortOrder(QString strName) const;

        QStringList getAllSortOrderNamesSorted() const;

        // Ingredients

        const Ingredient& addIngredient(QString strName, const Category& rCategory, Unit defaultUnit);
        bool existsIngredient(QString strName) const;
        void removeIngredient(const Ingredient& rIngredient);
        Ingredient& getIngredient(QString strName);
        const Ingredient& getIngredient(QString strName) const;

        QStringList getAllIngredientNamesSorted() const;

        // Recipes

        const Recipe& addRecipe(QString strName, quint32 iNrPersons);
        bool existsRecipe(QString strName) const;
        void removeRecipe(const Recipe& rRecipe);
        const Recipe& copyRecipe(const Recipe& rRecipe, QString strNewName);
        Recipe& getRecipe(QString strName);
        const Recipe& getRecipe(QString strName) const;

        QStringList getAllRecipeNamesSorted() const;

        // ShoppingList

        const ShoppingRecipe& addNewShoppingRecipe(QString strName);
        const ShoppingRecipe& addShoppingRecipe(QString strName, const Recipe& rRecipe);
        bool existsShoppingRecipe(QString strName) const;
        void removeShoppingRecipe(const ShoppingRecipe& rRecipe);
        ShoppingRecipe& getShoppingRecipe(QString strName);
        const ShoppingRecipe& getShoppingRecipe(QString strName) const;

        QStringList getAllShoppingRecipeNames() const;
        void clearShoppingList();

    private:
        QVector<QSharedPointer<Category>>       m_Categories;
        QVector<QSharedPointer<SortOrder>>      m_SortOrders;
        QVector<QSharedPointer<Ingredient>>     m_Ingredients;
        QVector<QSharedPointer<Recipe>>         m_Recipes;
        QVector<QSharedPointer<ShoppingRecipe>> m_ShoppingRecipes;
    };
}

#endif
