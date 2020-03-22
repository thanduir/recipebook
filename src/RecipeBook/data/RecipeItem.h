#ifndef RECIPEBOOK_RECIPE_ITEM_H
#define RECIPEBOOK_RECIPE_ITEM_H

#include <QString>
#include "Amount.h"
#include "Ingredient.h"
#include "Size.h"

namespace recipebook
{
    class RecipeItem
    {
    public:
        QString getName() const { return m_pIngredient->getName(); }
        const Ingredient& getIngredient() const { return *m_pIngredient; }
        void setIngredient(const Ingredient& rIngredient) { m_pIngredient = &rIngredient; }

        const Amount& getAmount() const { return m_Amount; }
        Amount& getAmount() { return m_Amount; }

        QString getAdditionalInfo() const { return m_AdditionalInfo; }
        void setAdditionInfo(QString additionalInfo) { m_AdditionalInfo = additionalInfo; }

        Size getSize() const { return m_Size; }
        void setSize(Size size) { m_Size = size; }

        bool isOptional() const { return m_bOptional; }
        void setIsOptional(bool optional) { m_bOptional = optional; }

    private:
        explicit RecipeItem(const Ingredient& rIngredient) : m_pIngredient(&rIngredient) {}
        RecipeItem(const RecipeItem& rOther);

        void operator=(const RecipeItem& rOther) = delete;

    private:
        const Ingredient* m_pIngredient;
        Amount m_Amount;
        QString m_AdditionalInfo;
        Size m_Size = Size::Normal;
        bool m_bOptional = false;

        friend class RecipeItemGroup;
    };
}

#endif
