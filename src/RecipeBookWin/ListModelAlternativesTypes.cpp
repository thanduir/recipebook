#include "ListModelAlternativesTypes.h"
#include "ListModelAlternativesGroups.h"
#include <data/RecipeBook.h>
#include <data/SortOrder.h>
#include <data/Ingredient.h>

using namespace recipebook::UI;

ListModelAlternativesTypes::ListModelAlternativesTypes()
{
}

bool ListModelAlternativesTypes::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    ListModelAlternativesGroups* pGroups = static_cast<ListModelAlternativesGroups*>(sourceModel());
    return pGroups->name(source_row) != pGroups->stringNoAlternativesGroup();
}

QString ListModelAlternativesTypes::name(int row) const
{
    ListModelAlternativesGroups* pGroups = static_cast<ListModelAlternativesGroups*>(sourceModel());
    int rowSource = mapToSource(index(row, 0)).row();
    return pGroups->name(rowSource);
}

int ListModelAlternativesTypes::renameType(int row, QString newName)
{
    ListModelAlternativesGroups* pGroups = static_cast<ListModelAlternativesGroups*>(sourceModel());
    int rowSource = mapToSource(index(row, 0)).row();
    return mapFromSource(pGroups->index(pGroups->renameType(rowSource, newName), 0)).row();
}

QString ListModelAlternativesTypes::color(int row) const
{
    ListModelAlternativesGroups* pGroups = static_cast<ListModelAlternativesGroups*>(sourceModel());
    int rowSource = mapToSource(index(row, 0)).row();
    return pGroups->color(rowSource);
}

void ListModelAlternativesTypes::setColor(int row, QString strColor)
{
    ListModelAlternativesGroups* pGroups = static_cast<ListModelAlternativesGroups*>(sourceModel());
    int rowSource = mapToSource(index(row, 0)).row();
    pGroups->setColor(rowSource, strColor);
}

int ListModelAlternativesTypes::addType(QString strType)
{
    ListModelAlternativesGroups* pGroups = static_cast<ListModelAlternativesGroups*>(sourceModel());
    return mapFromSource(pGroups->index(pGroups->addType(strType), 0)).row();
}

bool ListModelAlternativesTypes::existsType(QString strType) const
{
    ListModelAlternativesGroups* pGroups = static_cast<ListModelAlternativesGroups*>(sourceModel());
    return pGroups->existsType(strType);
}

bool ListModelAlternativesTypes::canTypeBeRemoved(int row) const
{
    ListModelAlternativesGroups* pGroups = static_cast<ListModelAlternativesGroups*>(sourceModel());
    int rowSource = mapToSource(index(row, 0)).row();
    return pGroups->canTypeBeRemoved(rowSource);
}

bool ListModelAlternativesTypes::removeType(int row)
{
    ListModelAlternativesGroups* pGroups = static_cast<ListModelAlternativesGroups*>(sourceModel());
    int rowSource = mapToSource(index(row, 0)).row();
    return pGroups->removeType(rowSource);
}
