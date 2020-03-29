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
    for(int i = 0; i < (int)rOther.getAlternativesGroupsCount(); ++i)
    {
        const RecipeItemGroup& rGroup = rOther.getAlternativesGroupAt(i);
        internal::unsorted::addItem(rGroup.getName(), i, m_ItemGroups, [&rGroup]()
        {
            return new RecipeItemGroup(rGroup);
        });
    }
}

RecipeItemGroup& Recipe::addAlternativesGroup(QString strName, int pos)
{
    return internal::unsorted::addItem(strName, pos, m_ItemGroups, [strName]()
    {
        return new RecipeItemGroup(strName);
    });
}

bool Recipe::existsAlternativesGroup(QString strName) const
{
    return internal::unsorted::exists<RecipeItemGroup>(strName, m_ItemGroups);
}

bool Recipe::removeAlternativesGroup(const RecipeItemGroup& rGroup)
{
    internal::unsorted::remove(rGroup, m_ItemGroups);
    return true;
}

RecipeItemGroup& Recipe::getAlternativesGroup(QString strName)
{
    return internal::unsorted::getItem(strName, m_ItemGroups);
}

const RecipeItemGroup& Recipe::getAlternativesGroup(QString strName) const
{
    return internal::unsorted::getItem(strName, m_ItemGroups);
}

quint32 Recipe::getAlternativesGroupsCount() const
{
    return m_ItemGroups.size();
}

RecipeItemGroup& Recipe::getAlternativesGroupAt(quint32 i)
{
    if(i >= (quint32)m_ItemGroups.size())
    {
        throw QException();
    }
    return *m_ItemGroups.at(i).get();
}

const RecipeItemGroup& Recipe::getAlternativesGroupAt(quint32 i) const
{
    if(i >= (quint32)m_ItemGroups.size())
    {
        throw QException();
    }
    return *m_ItemGroups.at(i).get();
}

void Recipe::moveAlternativesGroup(const RecipeItemGroup& rItem, quint32 newPos)
{
    int oldPos = internal::unsorted::find(rItem.getName(), m_ItemGroups);
    if(oldPos < 0)
    {
        throw QException();
    }
    m_ItemGroups.move(oldPos, newPos);
}
