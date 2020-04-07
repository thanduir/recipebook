#include "ListModelSortOrder.h"
#include "ListModelCategories.h"
#include <data/RecipeBook.h>
#include <data/Category.h>
#include <data/Ingredient.h>
#include <data/SortOrder.h>

using namespace recipebook::UI;

ListModelSortOrder::ListModelSortOrder(recipebook::RecipeBook& rRecipeBook)
:	m_rRecipeBook(rRecipeBook),
    m_pSortOrder(nullptr)
{
}

void ListModelSortOrder::setSortOrder(int row)
{
    if (row < 0 || row >= (int)m_rRecipeBook.getSortOrdersCount())
        return;

    beginResetModel();
    m_pSortOrder = &m_rRecipeBook.getSortOrderAt(row);
    endResetModel();

    sort(0, Qt::AscendingOrder);
}

bool ListModelSortOrder::lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const
{
    ListModelCategories* pCategories = static_cast<ListModelCategories*>(sourceModel());

    const Category& rCategoryLeft = m_rRecipeBook.getCategoryAt(source_left.row()); 
    const Category& rCategoryRight = m_rRecipeBook.getCategoryAt(source_right.row()); 

    if(m_pSortOrder == nullptr)
    {
        return source_left.row() < source_right.row();
    }

    return m_pSortOrder->getIndex(rCategoryLeft) < m_pSortOrder->getIndex(rCategoryRight);
}

QString ListModelSortOrder::name(int row) const
{
    ListModelCategories* pCategories = static_cast<ListModelCategories*>(sourceModel());
    int rowSource = mapToSource(index(row, 0)).row();
    return pCategories->name(rowSource);
}

int ListModelSortOrder::renameCategory(int row, QString newName)
{
    ListModelCategories* pCategories = static_cast<ListModelCategories*>(sourceModel());
    int rowSource = mapToSource(index(row, 0)).row();
    return mapFromSource(pCategories->index(pCategories->renameCategory(rowSource, newName), 0)).row();
}

int ListModelSortOrder::addCategory(QString strCategory)
{
    ListModelCategories* pCategories = static_cast<ListModelCategories*>(sourceModel());
    return mapFromSource(pCategories->index(pCategories->addCategory(strCategory), 0)).row();
}

bool ListModelSortOrder::existsCategory(QString strCategory) const
{
    ListModelCategories* pCategories = static_cast<ListModelCategories*>(sourceModel());
    return pCategories->existsCategory(strCategory);
}

QString ListModelSortOrder::listUsedInIngredients(int row) const
{
    ListModelCategories* pCategories = static_cast<ListModelCategories*>(sourceModel());
    int rowSource = mapToSource(index(row, 0)).row();
    return pCategories->listUsedInIngredients(rowSource);
}

bool ListModelSortOrder::canCategoryBeRemoved(int row) const
{
    ListModelCategories* pCategories = static_cast<ListModelCategories*>(sourceModel());
    int rowSource = mapToSource(index(row, 0)).row();
    return pCategories->canCategoryBeRemoved(rowSource);
}

bool ListModelSortOrder::removeCategory(int row)
{
    ListModelCategories* pCategories = static_cast<ListModelCategories*>(sourceModel());
    int rowSource = mapToSource(index(row, 0)).row();
    return pCategories->removeCategory(rowSource);
}
