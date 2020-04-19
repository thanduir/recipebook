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
    for(quint32 i = 0; i < rRecipe.getRecipeItemsCount(); ++i)
    {
        const RecipeItem& rItem = rRecipe.getRecipeItemAt(i);
        internal::unsorted::addItem(rItem.getName(), i, m_Items, [&rItem]()
        {
            return new ShoppingListItem(rItem);
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

 ShoppingListItem& ShoppingRecipe::addItem(const Ingredient& rIngredient)
{
    return internal::unsorted::addItem(rIngredient.getName(), -1, m_Items, [&rIngredient]()
    {
        return new ShoppingListItem(rIngredient);
    });
}

ShoppingListItem& ShoppingRecipe::addItem(const RecipeItem& rItem)
{
    return internal::unsorted::addItem(rItem.getName(), -1, m_Items, [&rItem]()
    {
        return new ShoppingListItem(rItem);
    });
}

bool ShoppingRecipe::existsItem(const Ingredient& rIngredient) const
{
    return internal::unsorted::exists<ShoppingListItem>(rIngredient.getName(), m_Items);
}

bool ShoppingRecipe::removeItem(const ShoppingListItem& rItem)
{
    internal::unsorted::remove(rItem, m_Items);
    return true;
}

ShoppingListItem& ShoppingRecipe::getItem(const Ingredient& rIngredient)
{
    return internal::unsorted::getItem(rIngredient.getName(), m_Items);
}

const ShoppingListItem& ShoppingRecipe::getItem(const Ingredient& rIngredient) const
{
    return internal::unsorted::getItem(rIngredient.getName(), m_Items);
}

quint32 ShoppingRecipe::getItemsCount() const
{
    return m_Items.size();
}

ShoppingListItem& ShoppingRecipe::getItemAt(quint32 i)
{
    if(i >= (quint32)m_Items.size())
    {
        throw QException();
    }
    return *m_Items.at(i).get();
}

const ShoppingListItem& ShoppingRecipe::getItemAt(quint32 i) const
{
    if(i >= (quint32)m_Items.size())
    {
        throw QException();
    }
    return *m_Items.at(i).get();
}

void ShoppingRecipe::moveItem(const ShoppingListItem& rItem, quint32 newPos)
{
    int oldPos = internal::unsorted::find(rItem.getName(), m_Items);
    if(oldPos < 0)
    {
        throw QException();
    }
    m_Items.move(oldPos, newPos);
}
