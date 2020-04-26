#include "SortModelSortOrder.h"
#include "ListModelCategories.h"
#include <data/RecipeBook.h>
#include <data/Category.h>
#include <data/Ingredient.h>
#include <data/SortOrder.h>
#include "RBDataHandler.h"

using namespace recipebook::UI;

SortModelSortOrder::SortModelSortOrder(recipebook::RBDataHandler& rRBDataHandler)
:	m_rRBDataHandler(rRBDataHandler),
	m_SortOrderIndex(-1)
{
}

void SortModelSortOrder::setSortOrder(int row)
{
	if(m_SortOrderIndex == row)
	{
		return;
	}

	beginResetModel();
	m_SortOrderIndex = row;
	endResetModel();
	
	sort(0, Qt::AscendingOrder);
}

bool SortModelSortOrder::lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const
{
	ListModelCategories* pCategories = static_cast<ListModelCategories*>(sourceModel());

	recipebook::RBDataReadHandle handle(m_rRBDataHandler);
	const Category& rCategoryLeft = handle.data().getCategoryAt(source_left.row()); 
	const Category& rCategoryRight = handle.data().getCategoryAt(source_right.row()); 

	if(m_SortOrderIndex < 0 || m_SortOrderIndex >= (int) handle.data().getSortOrdersCount())
	{
		return source_left.row() < source_right.row();
	}
	const SortOrder& rOrder = handle.data().getSortOrderAt(m_SortOrderIndex);
	return rOrder.getIndex(rCategoryLeft) < rOrder.getIndex(rCategoryRight);
}

QString SortModelSortOrder::name(int row) const
{
	ListModelCategories* pCategories = static_cast<ListModelCategories*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	return pCategories->name(rowSource);
}

int SortModelSortOrder::renameCategory(int row, QString newName)
{
	ListModelCategories* pCategories = static_cast<ListModelCategories*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	return mapFromSource(pCategories->index(pCategories->renameCategory(rowSource, newName), 0)).row();
}

int SortModelSortOrder::addCategory(QString strCategory)
{
	ListModelCategories* pCategories = static_cast<ListModelCategories*>(sourceModel());
	return mapFromSource(pCategories->index(pCategories->addCategory(strCategory), 0)).row();
}

bool SortModelSortOrder::existsCategory(QString strCategory) const
{
	ListModelCategories* pCategories = static_cast<ListModelCategories*>(sourceModel());
	return pCategories->existsCategory(strCategory);
}

QString SortModelSortOrder::listUsedInIngredients(int row) const
{
	ListModelCategories* pCategories = static_cast<ListModelCategories*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	return pCategories->listUsedInIngredients(rowSource);
}

bool SortModelSortOrder::canCategoryBeRemoved(int row) const
{
	ListModelCategories* pCategories = static_cast<ListModelCategories*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	return pCategories->canCategoryBeRemoved(rowSource);
}

bool SortModelSortOrder::removeCategory(int row)
{
	ListModelCategories* pCategories = static_cast<ListModelCategories*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	return pCategories->removeCategory(rowSource);
}

void SortModelSortOrder::moveCategory(int row, int target)
{
	if(row == target)
	{
		return;
	}

	recipebook::RBDataWriteHandle handle(m_rRBDataHandler);
	if(m_SortOrderIndex >= 0 && m_SortOrderIndex < (int) handle.data().getSortOrdersCount())
	{
		SortOrder& rOrder = handle.data().getSortOrderAt(m_SortOrderIndex);
		rOrder.moveCategory(rOrder.at(row), target);
	}
}
