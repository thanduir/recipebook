#include "FilterModelIngredients.h"
#include "ListModelIngredients.h"
#include <data/RecipeBook.h>
#include <data/SortOrder.h>
#include <data/Ingredient.h>

using namespace recipebook::UI;

FilterModelIngredients::FilterModelIngredients()
{
}

void FilterModelIngredients::setFilterString(QString strFilter)
{
    if(m_FilterString != strFilter)
    {
        m_FilterString = strFilter;
        invalidateFilter();
    }
}

bool FilterModelIngredients::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    ListModelIngredients* pIngredients = static_cast<ListModelIngredients*>(sourceModel());
    return pIngredients->name(source_row).contains(m_FilterString, Qt::CaseInsensitive);
}

QString FilterModelIngredients::name(int row) const
{
    ListModelIngredients* pIngredients = static_cast<ListModelIngredients*>(sourceModel());
    int rowSource = mapToSource(index(row, 0)).row();
    return pIngredients->name(rowSource);
}
