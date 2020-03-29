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

        void clearData();

        // Categories

        Category& addCategory(QString strName);
        void renameCategory(Category& rCategory, QString strNewName);
        bool existsCategory(QString strName) const;
        bool removeCategory(const Category& rCategory);        
        const Category& getCategory(QString strName) const;
        const Category& getDefaultCategory() const;
        quint32 getCategoriesCount() const;
        Category& getCategoryAt(quint32 i);
        const Category& getCategoryAt(quint32 i) const;

        // SortOrders
        
        SortOrder& addSortOrder(QString strName);
        void renameSortOrder(SortOrder& rOrder, QString strNewName);
        bool existsSortOrder(QString strName) const;
        bool removeSortOrder(const SortOrder& rOrder);
        SortOrder& getSortOrder(QString strName);
        const SortOrder& getSortOrder(QString strName) const;
        quint32 getSortOrdersCount() const;
        SortOrder& getSortOrderAt(quint32 i);
        const SortOrder& getSortOrderAt(quint32 i) const;

        // Ingredients

        Ingredient& addIngredient(QString strName, const Category& rCategory, Unit defaultUnit);
        void renameIngredient(Ingredient& rIngredient, QString strNewName);
        bool removeIngredient(const Ingredient& rIngredient);
        bool existsIngredient(QString strName) const;
        Ingredient& getIngredient(QString strName);
        const Ingredient& getIngredient(QString strName) const;
        quint32 getIngredientsCount() const;
        Ingredient& getIngredientAt(quint32 i);
        const Ingredient& getIngredientAt(quint32 i) const;

        // Recipes
        
        Recipe& addRecipe(QString strName, quint32 uiNrPersons);
        void renameRecipe(Recipe& rRecipe, QString strNewName);
        bool existsRecipe(QString strName) const;
        bool removeRecipe(const Recipe& rRecipe);
        const Recipe& copyRecipe(const Recipe& rRecipe, QString strNewName);
        Recipe& getRecipe(QString strName);
        const Recipe& getRecipe(QString strName) const;
        quint32 getRecipesCount() const;
        Recipe& getRecipeAt(quint32 i);
        const Recipe& getRecipeAt(quint32 i) const;

        // ShoppingList
        
        ShoppingRecipe& addNewShoppingRecipe(QString strName, float fScalingFactor);
        ShoppingRecipe& addShoppingRecipe(QString strName, const Recipe& rRecipe);
        void renameShoppingRecipe(ShoppingRecipe& rRecipe, QString strNewName);
        bool existsShoppingRecipe(QString strName) const;
        bool removeShoppingRecipe(const ShoppingRecipe& rRecipe);
        ShoppingRecipe& getShoppingRecipe(QString strName);
        const ShoppingRecipe& getShoppingRecipe(QString strName) const;
        quint32 getShoppingRecipesCount() const;
        ShoppingRecipe& getShoppingRecipeAt(quint32 i);
        const ShoppingRecipe& getShoppingRecipeAt(quint32 i) const;

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