#include "ListModelSortOrders.h"
#include "ListModelProvenance.h"
#include <data/RecipeBook.h>
#include <data/SortOrder.h>
#include <data/Ingredient.h>

using namespace recipebook::UI;

ListModelSortOrders::ListModelSortOrders()
{
}

bool ListModelSortOrders::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    ListModelProvenance* pProvenances = static_cast<ListModelProvenance*>(sourceModel());
    return pProvenances->name(source_row) != pProvenances->provenanceEverywhere();
}

QString ListModelSortOrders::name(int row) const
{
    ListModelProvenance* pProvenances = static_cast<ListModelProvenance*>(sourceModel());
    int rowSource = mapToSource(index(row, 0)).row();
    return pProvenances->name(rowSource);
}

int ListModelSortOrders::renameSortOrder(int row, QString newName)
{
    ListModelProvenance* pProvenances = static_cast<ListModelProvenance*>(sourceModel());
    int rowSource = mapToSource(index(row, 0)).row();
    return mapFromSource(pProvenances->index(pProvenances->renameProvenance(rowSource, newName), 0)).row();
}

int ListModelSortOrders::addSortOrder(QString strSortOrder)
{
    ListModelProvenance* pProvenances = static_cast<ListModelProvenance*>(sourceModel());
    return mapFromSource(pProvenances->index(pProvenances->addProvenance(strSortOrder), 0)).row();
}

bool ListModelSortOrders::existsSortOrder(QString strSortOrder) const
{
    ListModelProvenance* pProvenances = static_cast<ListModelProvenance*>(sourceModel());
    return pProvenances->existsProvenance(strSortOrder);
}

QString ListModelSortOrders::listUsedInIngredients(int row) const
{
    ListModelProvenance* pProvenances = static_cast<ListModelProvenance*>(sourceModel());
    int rowSource = mapToSource(index(row, 0)).row();
    return pProvenances->listUsedInIngredients(rowSource);
}

bool ListModelSortOrders::canSortOrderBeRemoved(int row) const
{
    ListModelProvenance* pProvenances = static_cast<ListModelProvenance*>(sourceModel());
    int rowSource = mapToSource(index(row, 0)).row();
    return pProvenances->canProvenanceBeRemoved(rowSource);
}

bool ListModelSortOrders::removeSortOrder(int row)
{
    ListModelProvenance* pProvenances = static_cast<ListModelProvenance*>(sourceModel());
    int rowSource = mapToSource(index(row, 0)).row();
    return pProvenances->removeProvenance(rowSource);
}
