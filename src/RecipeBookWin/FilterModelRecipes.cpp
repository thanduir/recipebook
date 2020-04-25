#include "FilterModelRecipes.h"
#include "ListModelRecipes.h"
#include <data/RecipeBook.h>
#include <data/SortOrder.h>
#include <data/Ingredient.h>

using namespace recipebook::UI;

FilterModelRecipes::FilterModelRecipes()
{
}

void FilterModelRecipes::setFilterString(QString strFilter)
{
	if(m_FilterString != strFilter)
	{
		m_FilterString = strFilter;
		m_AdditionalItemsToShow.clear();
		invalidateFilter();
	}
}

bool FilterModelRecipes::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
	ListModelRecipes* pRecipes = static_cast<ListModelRecipes*>(sourceModel());
	QString itemName = pRecipes->name(source_row);
	return itemName.contains(m_FilterString, Qt::CaseInsensitive) || m_AdditionalItemsToShow.contains(itemName);
}

QString FilterModelRecipes::name(int row) const
{
	ListModelRecipes* pRecipes = static_cast<ListModelRecipes*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	return pRecipes->name(rowSource);
}

quint32 FilterModelRecipes::numberOfPersons(int row) const
{
	ListModelRecipes* pRecipes = static_cast<ListModelRecipes*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	return pRecipes->numberOfPersons(rowSource);
}

QString FilterModelRecipes::shortDescription(int row) const
{
	ListModelRecipes* pRecipes = static_cast<ListModelRecipes*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	return pRecipes->shortDescription(rowSource);
}

QString FilterModelRecipes::recipeText(int row) const
{
	ListModelRecipes* pRecipes = static_cast<ListModelRecipes*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	return pRecipes->recipeText(rowSource);
}

quint32 FilterModelRecipes::cookingTime(int row) const
{
	ListModelRecipes* pRecipes = static_cast<ListModelRecipes*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	return pRecipes->cookingTime(rowSource);
}

bool FilterModelRecipes::isEverythingSet(int row) const
{
	ListModelRecipes* pRecipes = static_cast<ListModelRecipes*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	return pRecipes->isEverythingSet(rowSource);
}

void FilterModelRecipes::setNumberOfPersons(int row, qint32 nrPersons)
{
	ListModelRecipes* pRecipes = static_cast<ListModelRecipes*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	pRecipes->setNumberOfPersons(rowSource, nrPersons);
}

void FilterModelRecipes::setShortDescription(int row, QString desc)
{
	ListModelRecipes* pRecipes = static_cast<ListModelRecipes*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	pRecipes->setShortDescription(rowSource, desc);
}

void FilterModelRecipes::setRecipeText(int row, QString text)
{
	ListModelRecipes* pRecipes = static_cast<ListModelRecipes*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	pRecipes->setRecipeText(rowSource, text);
}

void FilterModelRecipes::setCookingTime(int row, quint32 timeInMin)
{
	ListModelRecipes* pRecipes = static_cast<ListModelRecipes*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	pRecipes->setCookingTime(rowSource, timeInMin);
}
        
int FilterModelRecipes::renameRecipe(int row, QString newName)
{
	ListModelRecipes* pRecipes = static_cast<ListModelRecipes*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	m_AdditionalItemsToShow.append(newName);
	return mapFromSource(pRecipes->index(pRecipes->renameRecipe(rowSource, newName), 0)).row();
}

int FilterModelRecipes::addRecipe(QString strRecipe)
{
	ListModelRecipes* pRecipes = static_cast<ListModelRecipes*>(sourceModel());
	m_AdditionalItemsToShow.append(strRecipe);
	return mapFromSource(pRecipes->index(pRecipes->addRecipe(strRecipe), 0)).row();
}

int FilterModelRecipes::copyRecipe(int row, QString strRecipe)
{
	ListModelRecipes* pRecipes = static_cast<ListModelRecipes*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	m_AdditionalItemsToShow.append(strRecipe);
	return mapFromSource(pRecipes->index(pRecipes->copyRecipe(rowSource, strRecipe), 0)).row();
}

bool FilterModelRecipes::existsRecipe(QString strRecipe) const
{
	ListModelRecipes* pRecipes = static_cast<ListModelRecipes*>(sourceModel());
	return pRecipes->existsRecipe(strRecipe);
}

bool FilterModelRecipes::removeRecipe(int row)
{
	ListModelRecipes* pRecipes = static_cast<ListModelRecipes*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	return pRecipes->removeRecipe(rowSource);
}
