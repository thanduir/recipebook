#include "FilterModelUnusedRecipes.h"
#include "ListModelRecipes.h"
#include <data/RecipeBook.h>
#include <data/SortOrder.h>
#include <data/Ingredient.h>
#include <data/RecipeBookConfiguration.h>
#include <data/RBDataHandler.h>

using namespace recipebook::UI;

FilterModelUnusedRecipes::FilterModelUnusedRecipes(RBDataHandler& rRBDataHandler)
:	m_rRBDataHandler(rRBDataHandler)
{
}

void FilterModelUnusedRecipes::setRecipeBookConfiguration(quint32 uiConfig)
{
	m_iConfig = (int)uiConfig;
	invalidateFilter();
}

void FilterModelUnusedRecipes::setFilterString(QString strFilter)
{
	if (m_FilterString != strFilter)
	{
		m_FilterString = strFilter;
		invalidateFilter();
	}
}

bool FilterModelUnusedRecipes::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
	Q_UNUSED(source_parent);

	ListModelRecipes* pRecipes = static_cast<ListModelRecipes*>(sourceModel());
	QString itemName = pRecipes->name(source_row);

	{
		RBDataReadHandle handle(m_rRBDataHandler);
		if (m_iConfig < 0 || m_iConfig >= (int)handle.data().getConfigurationsCount())
		{
			return false;
		}
		
        const RecipeBookConfiguration& rConfig = handle.data().getConfigurationAt((quint32)m_iConfig);
		if(rConfig.existsRecipe(handle.data().getRecipe(itemName)))
		{
			return false;
		}
	}

	return itemName.contains(m_FilterString, Qt::CaseInsensitive);
}
