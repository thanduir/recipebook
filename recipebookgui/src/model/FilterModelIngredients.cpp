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
	Q_UNUSED(source_parent);

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

bool FilterModelIngredients::provenanceAvailable(int row, QString strProvenance) const
{
	ListModelIngredients* pIngredients = static_cast<ListModelIngredients*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	return pIngredients->provenanceAvailable(rowSource, strProvenance);
}

QString FilterModelIngredients::defaultUnit(int row) const
{
	ListModelIngredients* pIngredients = static_cast<ListModelIngredients*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	return pIngredients->defaultUnit(rowSource);
}

QStringList FilterModelIngredients::listUsedInRecipes(int row) const
{
	ListModelIngredients* pIngredients = static_cast<ListModelIngredients*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	return pIngredients->listUsedInRecipes(rowSource);
}

QStringList FilterModelIngredients::listUsedInShoppingRecipes(int row) const
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

void FilterModelIngredients::setProvenanceAvailable(int row, QString strProvenance, bool bAvailable)
{
	ListModelIngredients* pIngredients = static_cast<ListModelIngredients*>(sourceModel());
	int rowSource = mapToSource(index(row, 0)).row();
	pIngredients->setProvenanceAvailable(rowSource, strProvenance, bAvailable);
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

bool FilterModelIngredients::canIngredientsBeAdded() const
{
	ListModelIngredients* pIngredients = static_cast<ListModelIngredients*>(sourceModel());
	return pIngredients->canIngredientsBeAdded();
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
