#ifndef RECIPEBOOK_RECIPE_ITEM_GROUP_H
#define RECIPEBOOK_RECIPE_ITEM_GROUP_H

#include <QString>
#include <QSharedPointer>

namespace recipebook
{
    class RecipeItem;
    class Ingredient;

    class RecipeItemGroup
    {
    public:
        QString getName() const { return m_Name; }
        void rename(QString strNewName) { m_Name = strNewName; }

        RecipeItem& addItem(const Ingredient& rIngredient, int pos = -1);
        bool existsItem(const Ingredient& rIngredient) const;
        bool removeItem(const RecipeItem& rItem);
        RecipeItem& getItem(const Ingredient& rIngredient);
        const RecipeItem& getItem(const Ingredient& rIngredient) const;

        
        quint32 getItemsCount() const;
        RecipeItem& getItemAt(quint32 i);
        const RecipeItem& getItemAt(quint32 i) const;
        void moveItem(const RecipeItem& rItem, quint32 newPos);

    private:
        explicit RecipeItemGroup(QString strName) : m_Name(strName) {}      
        RecipeItemGroup(const RecipeItemGroup& rOther);

        void operator=(const RecipeItemGroup& rOther) = delete;

    private:
        QString m_Name;
        QVector<QSharedPointer<RecipeItem>> m_Items;

        friend class Recipe;
        friend class ShoppingRecipe;
    };
}

#endif