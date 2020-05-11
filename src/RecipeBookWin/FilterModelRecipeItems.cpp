#include "FilterModelRecipeItems.h"
#include "ListModelRecipeItems.h"
#include <data/RecipeBook.h>

using namespace recipebook::UI;

FilterModelRecipeItems::FilterModelRecipeItems()
{
}

bool FilterModelRecipeItems::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
	Q_UNUSED(source_parent);

	ListModelRecipeItems* pItems = static_cast<ListModelRecipeItems*>(sourceModel());
	return !pItems->hasGroup(source_row) || pItems->firstInGroup(source_row);
}

void FilterModelRecipeItems::beginMove(int row)
{
	m_MoveFrom = row;
	m_MoveTo = row;
}

void FilterModelRecipeItems::updateMoveTarget(int target)
{
	m_MoveTo = target;
}

void FilterModelRecipeItems::applyMove()
{
	if(m_MoveFrom != m_MoveTo)
	{
		ListModelRecipeItems* pItems = static_cast<ListModelRecipeItems*>(sourceModel());
		int fromSource = mapToSource(index(m_MoveFrom, 0)).row();
		int toSource = mapToSource(index(m_MoveTo, 0)).row();
		pItems->moveItem(fromSource, toSource);
	}

	m_MoveFrom = -1;
	m_MoveTo = -1;
}
