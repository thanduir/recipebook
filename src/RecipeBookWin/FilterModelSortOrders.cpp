#include "FilterModelSortOrders.h"
#include "ListModelProvenance.h"
#include <data/RecipeBook.h>
#include <data/SortOrder.h>
#include <data/Ingredient.h>

using namespace recipebook::UI;

FilterModelSortOrders::FilterModelSortOrders()
{
}

bool FilterModelSortOrders::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    ListModelProvenance* pProvenances = static_cast<ListModelProvenance*>(sourceModel());
    return pProvenances->name(source_row) != pProvenances->provenanceEverywhere();
}

QString FilterModelSortOrders::name(int row) const
{
    ListModelProvenance* pProvenances = static_cast<ListModelProvenance*>(sourceModel());
    int rowSource = mapToSource(index(row, 0)).row();
    return pProvenances->name(rowSource);
}

int FilterModelSortOrders::renameSortOrder(int row, QString newName)
{
    ListModelProvenance* pProvenances = static_cast<ListModelProvenance*>(sourceModel());
    int rowSource = mapToSource(index(row, 0)).row();
    return mapFromSource(pProvenances->index(pProvenances->renameProvenance(rowSource, newName), 0)).row();
}

int FilterModelSortOrders::addSortOrder(QString strSortOrder)
{
    ListModelProvenance* pProvenances = static_cast<ListModelProvenance*>(sourceModel());
    return mapFromSource(pProvenances->index(pProvenances->addProvenance(strSortOrder), 0)).row();
}

bool FilterModelSortOrders::existsSortOrder(QString strSortOrder) const
{
    ListModelProvenance* pProvenances = static_cast<ListModelProvenance*>(sourceModel());
    return pProvenances->existsProvenance(strSortOrder);
}

QString FilterModelSortOrders::listUsedInIngredients(int row) const
{
    ListModelProvenance* pProvenances = static_cast<ListModelProvenance*>(sourceModel());
    int rowSource = mapToSource(index(row, 0)).row();
    return pProvenances->listUsedInIngredients(rowSource);
}

bool FilterModelSortOrders::canSortOrderBeRemoved(int row) const
{
    ListModelProvenance* pProvenances = static_cast<ListModelProvenance*>(sourceModel());
    int rowSource = mapToSource(index(row, 0)).row();
    return pProvenances->canProvenanceBeRemoved(rowSource);
}

bool FilterModelSortOrders::removeSortOrder(int row)
{
    ListModelProvenance* pProvenances = static_cast<ListModelProvenance*>(sourceModel());
    int rowSource = mapToSource(index(row, 0)).row();
    return pProvenances->removeProvenance(rowSource);
}
