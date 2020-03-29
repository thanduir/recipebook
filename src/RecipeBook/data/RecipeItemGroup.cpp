#include "RecipeItemGroup.h"
#include "Ingredient.h"
#include "RecipeItem.h"
#include "util/ItemsUtil.h"

using namespace recipebook;

RecipeItemGroup::RecipeItemGroup(const RecipeItemGroup& rOther)
:   m_Name(rOther.m_Name)
{
    for(int i = 0; i < rOther.m_Items.size(); ++i)
    {
        const RecipeItem& rItem = rOther.getItemAt(i);
        internal::unsorted::addItem(rItem.getName(), i, m_Items, [&rItem]()
        {
            return new RecipeItem(rItem);
        });
    }
}

RecipeItem& RecipeItemGroup::addItem(const Ingredient& rIngredient, int pos)
{
	return internal::unsorted::addItem(rIngredient.getName(), pos, m_Items, [&rIngredient]()
    {
        return new RecipeItem(rIngredient);
    });
}

bool RecipeItemGroup::existsItem(const Ingredient& rIngredient) const
{
    return internal::unsorted::exists<RecipeItem>(rIngredient.getName(), m_Items);
}

bool RecipeItemGroup::removeItem(const RecipeItem& rItem)
{
    internal::unsorted::remove(rItem, m_Items);
    return true;
}

RecipeItem& RecipeItemGroup::getItem(const Ingredient& rIngredient)
{
    return internal::unsorted::getItem(rIngredient.getName(), m_Items);
}

const RecipeItem& RecipeItemGroup::getItem(const Ingredient& rIngredient) const
{
    return internal::unsorted::getItem(rIngredient.getName(), m_Items);
}

quint32 RecipeItemGroup::getItemsCount() const
{
    return m_Items.size();
}

RecipeItem& RecipeItemGroup::getItemAt(quint32 i)
{
    if(i >= (quint32)m_Items.size())
    {
        throw QException();
    }
    return *m_Items.at(i).get();
}

const RecipeItem& RecipeItemGroup::getItemAt(quint32 i) const
{
    if(i >= (quint32)m_Items.size())
    {
        throw QException();
    }
    return *m_Items.at(i).get();
}

void RecipeItemGroup::moveItem(const RecipeItem& rItem, quint32 newPos)
{
    int oldPos = internal::unsorted::find(rItem.getName(), m_Items);
    if(oldPos < 0)
    {
        throw QException();
    }
    m_Items.move(oldPos, newPos);
}
