#include "ShoppingRecipe.h"
#include "Ingredient.h"
#include "Recipe.h"
#include "RecipeItem.h"
#include "ShoppingListItem.h"
#include "util/ItemsUtil.h"

using namespace recipebook;

ShoppingRecipe::ShoppingRecipe(const Recipe& rRecipe)
:   m_Name(rRecipe.getName()), 
    m_fScalingFactor(rRecipe.getNumberOfPersons())
{
    const RecipeItemGroup& rGroup = rRecipe.getRecipeItems();
    for(QSharedPointer<RecipeItem> spItem : qAsConst(rGroup.m_Items))
    {
        internal::addItem(spItem->getName(), m_Items, [&spItem]()
        {
            return new ShoppingListItem(*spItem.get());
        });
    }
}

void ShoppingRecipe::changeScalingFactor(float f)
{
	setScalingFactor(f);

    float fFactor = f / getScalingFactor();
    for(QSharedPointer<ShoppingListItem> spItem : qAsConst(m_Items))
    {
        Amount& amount = spItem->getAmount();
        amount.scaleAmount(fFactor);
    }
}

const ShoppingListItem& ShoppingRecipe::addItem(const Ingredient& rIngredient)
{
    return internal::addItem(rIngredient.getName(), m_Items, [&rIngredient]()
    {
        return new ShoppingListItem(rIngredient);
    });
}

const ShoppingListItem& ShoppingRecipe::addItem(const RecipeItem& rItem)
{
    return internal::addItem(rItem.getName(), m_Items, [&rItem]()
    {
        return new ShoppingListItem(rItem);
    });
}

bool ShoppingRecipe::existsItem(const Ingredient& rIngredient) const
{
    return internal::exists<ShoppingListItem>(rIngredient.getName(), m_Items);
}

bool ShoppingRecipe::removeItem(const ShoppingListItem& rItem)
{
    internal::remove(rItem, m_Items);
    return true;
}

ShoppingListItem& ShoppingRecipe::getItem(const Ingredient& rIngredient)
{
    return internal::getItem(rIngredient.getName(), m_Items);
}

const ShoppingListItem& ShoppingRecipe::getItem(const Ingredient& rIngredient) const
{
    return internal::getItemConst(rIngredient.getName(), m_Items);
}

QStringList ShoppingRecipe::getAllItemNamesSorted() const
{
    return internal::getAllNames(m_Items);
}
