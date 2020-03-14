#include "Recipe.h"
#include "RecipeItem.h"
#include "RecipeItemGroup.h"
#include "util/ItemsUtil.h"

using namespace recipebook;

Recipe::Recipe(QString strName, const Recipe& rOther)
:   m_Name(strName),
    m_NrPersons(rOther.getNumberOfPersons()),
    m_RecipeItems(rOther.m_RecipeItems)
{
    for(QSharedPointer<RecipeItemGroup> spGroup : qAsConst(rOther.m_ItemGroups))
    {
        internal::addItem(spGroup->getName(), m_ItemGroups, [&spGroup]()
        {
            return new RecipeItemGroup(*spGroup.get());
        });
    }
}

const RecipeItemGroup& Recipe::addAlternativesGroup(QString strName)
{
    return internal::addItem(strName, m_ItemGroups, [strName]()
    {
        return new RecipeItemGroup(strName);
    });
}

bool Recipe::existsAlternativesGroup(QString strName) const
{
    return internal::exists<RecipeItemGroup>(strName, m_ItemGroups);
}

bool Recipe::removeAlternativesGroup(const RecipeItemGroup& rGroup)
{
    internal::remove(rGroup, m_ItemGroups);
    return true;
}

RecipeItemGroup& Recipe::getAlternativesGroup(QString strName)
{
    return internal::getItem(strName, m_ItemGroups);
}

const RecipeItemGroup& Recipe::getAlternativesGroup(QString strName) const
{
    return internal::getItemConst(strName, m_ItemGroups);
}

QStringList Recipe::getAllAlternativesGroupsNamesSorted() const
{
    return internal::getAllNames(m_ItemGroups);
}
