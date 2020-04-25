#include "SortModelSortOrder.h"
#include "ListModelCategories.h"
#include <data/RecipeBook.h>
#include <data/Category.h>
#include <data/Ingredient.h>
#include <data/SortOrder.h>

using namespace recipebook::UI;

SortModelSortOrder::SortModelSortOrder(recipebook::RecipeBook& rRecipeBook)
:	m_rRecipeBook(rRecipeBook),
	m_pSortOrder(nullptr)
{
}

void SortModelSortOrder::setSortOrder(int row)
{
	if(row < 0 || row >= (int) m_rRecipeBook.getSortOrdersCount())
	{
		beginResetModel();
		m_pSortOrder = nullptr;
		endResetModel();
		return;
	}

	beginResetModel();
	m_pSortOrder = &m_rRecipeBook.getSortOrderAt(row);
	endResetModel();

	sort(0, Qt::AscendingOrder);
}

bool SortModelSortOrder::lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const
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

	m_pSortOrder->moveCategory(m_pSortOrder->at(row), target);
}
