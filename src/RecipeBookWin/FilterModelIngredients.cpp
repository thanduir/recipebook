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
		m_AdditionalItemsToShow.clear();
		invalidateFilter();
	}
}

bool FilterModelIngredients::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
	ListModelIngredients* pIngredients = static_cast<ListModelIngredients*>(sourceModel());
	QString itemName = pIngredients->name(source_row);
	return itemName.contains(m_FilterString, Qt::CaseInsensitive) || m_AdditionalItemsToShow.contains(itemName);
}

QString FilterModelIngredients::name(int row) const
{
	ListModelIngredients* pIngredients = static_cast<ListModelIngredients*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	return pIngredients->name(rowSource);
}

QString FilterModelIngredients::category(int row) const
{
	ListModelIngredients* pIngredients = static_cast<ListModelIngredients*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	return pIngredients->category(rowSource);
}

QString FilterModelIngredients::provenance(int row) const
{
	ListModelIngredients* pIngredients = static_cast<ListModelIngredients*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	return pIngredients->provenance(rowSource);
}

QString FilterModelIngredients::defaultUnit(int row) const
{
	ListModelIngredients* pIngredients = static_cast<ListModelIngredients*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	return pIngredients->defaultUnit(rowSource);
}

QString FilterModelIngredients::listUsedInRecipes(int row) const
{
	ListModelIngredients* pIngredients = static_cast<ListModelIngredients*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	return pIngredients->listUsedInRecipes(rowSource);
}

QString FilterModelIngredients::listUsedInShoppingRecipes(int row) const
{
	ListModelIngredients* pIngredients = static_cast<ListModelIngredients*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	return pIngredients->listUsedInShoppingRecipes(rowSource);
}

void FilterModelIngredients::setCategory(int row, QString newCategory)
{
	ListModelIngredients* pIngredients = static_cast<ListModelIngredients*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	pIngredients->setCategory(rowSource, newCategory);
}

void FilterModelIngredients::setProvenance(int row, QString newProvenance)
{
	ListModelIngredients* pIngredients = static_cast<ListModelIngredients*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	pIngredients->setProvenance(rowSource, newProvenance);
}

void FilterModelIngredients::setDefaultUnit(int row, QString newDefaultUnit)
{
	ListModelIngredients* pIngredients = static_cast<ListModelIngredients*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	pIngredients->setDefaultUnit(rowSource, newDefaultUnit);
}

int FilterModelIngredients::renameIngredient(int row, QString newName)
{
	ListModelIngredients* pIngredients = static_cast<ListModelIngredients*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	m_AdditionalItemsToShow.append(newName);
	return mapFromSource(pIngredients->index(pIngredients->renameIngredient(rowSource, newName), 0)).row();
}

int FilterModelIngredients::addIngredient(QString strIngredient)
{
	ListModelIngredients* pIngredients = static_cast<ListModelIngredients*>(sourceModel());
	m_AdditionalItemsToShow.append(strIngredient);
	return mapFromSource(pIngredients->index(pIngredients->addIngredient(strIngredient), 0)).row();
}

bool FilterModelIngredients::existsIngredient(QString strIngredient) const
{
	ListModelIngredients* pIngredients = static_cast<ListModelIngredients*>(sourceModel());
	return pIngredients->existsIngredient(strIngredient);
}

bool FilterModelIngredients::canIngredientBeRemoved(int row) const
{
	ListModelIngredients* pIngredients = static_cast<ListModelIngredients*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	return pIngredients->canIngredientBeRemoved(rowSource);
}

bool FilterModelIngredients::removeIngredient(int row)
{
	ListModelIngredients* pIngredients = static_cast<ListModelIngredients*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	return pIngredients->removeIngredient(rowSource);
}
