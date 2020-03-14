#include "RecipeItemGroup.h"
#include "Ingredient.h"
#include "RecipeItem.h"
#include "util/ItemsUtil.h"

using namespace recipebook;

RecipeItemGroup::RecipeItemGroup(const RecipeItemGroup& rOther)
:   m_Name(rOther.m_Name)
{
    for(QSharedPointer<RecipeItem> spItem : qAsConst(rOther.m_Items))
    {
        internal::addItem(spItem->getName(), m_Items, [&spItem]()
        {
            return new RecipeItem(*spItem.get());
        });
    }
}

const RecipeItem& RecipeItemGroup::addItem(const Ingredient& rIngredient)
{
	return internal::addItem(rIngredient.getName(), m_Items, [&rIngredient]()
    {
        return new RecipeItem(rIngredient);
    });
}

bool RecipeItemGroup::existsItem(const Ingredient& rIngredient) const
{
    return internal::exists<RecipeItem>(rIngredient.getName(), m_Items);
}

bool RecipeItemGroup::removeItem(const RecipeItem& rItem)
{
    internal::remove(rItem, m_Items);
    return true;
}

RecipeItem& RecipeItemGroup::getItem(const Ingredient& rIngredient)
{
    return internal::getItem(rIngredient.getName(), m_Items);
}

const RecipeItem& RecipeItemGroup::getItem(const Ingredient& rIngredient) const
{
    return internal::getItemConst(rIngredient.getName(), m_Items);
}

QStringList RecipeItemGroup::getAllItemNamesSorted() const
{
    return internal::getAllNames(m_Items);
}
