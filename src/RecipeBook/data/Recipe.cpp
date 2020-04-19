#include "Recipe.h"
#include "RecipeItem.h"
#include "Ingredient.h"
#include "util/ItemsUtil.h"

using namespace recipebook;

Recipe::Recipe(QString strName, const Recipe& rOther)
:   m_Name(strName),
    m_NrPersons(rOther.getNumberOfPersons()),
    m_RecipeItems(rOther.m_RecipeItems)
{
    for(int i = 0; i < rOther.m_RecipeItems.size(); ++i)
    {
        const RecipeItem& rItem = rOther.getRecipeItemAt(i);
        internal::unsorted::addItem(rItem.getName(), i, m_RecipeItems, [&rItem]()
        {
            return new RecipeItem(rItem);
        });
    }
}

RecipeItem& Recipe::addRecipeItem(const Ingredient& rIngredient)
{
	return internal::unsorted::addItem(rIngredient.getName(), -1, m_RecipeItems, [&rIngredient]()
    {
        return new RecipeItem(rIngredient);
    });
}

bool Recipe::existsRecipeItem(const Ingredient& rIngredient) const
{
    return internal::unsorted::exists<RecipeItem>(rIngredient.getName(), m_RecipeItems);
}

bool Recipe::removeRecipeItem(const RecipeItem& rItem)
{
    internal::unsorted::remove(rItem, m_RecipeItems);
    return true;
}

RecipeItem& Recipe::getRecipeItem(const Ingredient& rIngredient)
{
    return internal::unsorted::getItem(rIngredient.getName(), m_RecipeItems);
}

const RecipeItem& Recipe::getRecipeItem(const Ingredient& rIngredient) const
{
    return internal::unsorted::getItem(rIngredient.getName(), m_RecipeItems);
}

quint32 Recipe::getRecipeItemsCount() const
{
    return m_RecipeItems.size();
}

RecipeItem& Recipe::getRecipeItemAt(quint32 i)
{
    if(i >= (quint32)m_RecipeItems.size())
    {
        throw QException();
    }
    return *m_RecipeItems.at(i).get();
}

const RecipeItem& Recipe::getRecipeItemAt(quint32 i) const
{
    if(i >= (quint32)m_RecipeItems.size())
    {
        throw QException();
    }
    return *m_RecipeItems.at(i).get();
}

void Recipe::moveRecipeItem(const RecipeItem& rItem, quint32 newPos)
{
    int oldPos = internal::unsorted::find(rItem.getName(), m_RecipeItems);
    if(oldPos < 0)
    {
        throw QException();
    }
    m_RecipeItems.move(oldPos, newPos);
}
