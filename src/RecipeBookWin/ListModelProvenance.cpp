#include "ListModelProvenance.h"
#include <data/RecipeBook.h>
#include <data/Ingredient.h>
#include <data/SortOrder.h>
#include "uistringconverter.h"
#include "RBDataHandler.h"

using namespace recipebook::UI;

const quint32 c_uiRowEverywhere = 0;

ListModelProvenance::ListModelProvenance(recipebook::RBDataHandler& rRBDataHandler, const UIStringConverter& rConverter)
:	m_rRBDataHandler(rRBDataHandler),
	m_rConverter(rConverter)
{
}

int ListModelProvenance::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);
	return handle.data().getSortOrdersCount() + 1;
}

QVariant ListModelProvenance::data(const QModelIndex& index, int iRole) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if (index.row() < 0 || index.row() >= (int)handle.data().getIngredientsCount())
		return QVariant();

	if(index.row() == c_uiRowEverywhere)
	{
		return m_rConverter.getProvenanceEverywhere();
	}

	const SortOrder& rSortOrder = handle.data().getSortOrderAt(index.row()-1);
	ProvenanceRoles role = static_cast<ProvenanceRoles>(iRole);
	if(role == ProvenanceRoles::NameRole)
	{
		return rSortOrder.getName();
	}

	return QVariant();
}

QHash<int, QByteArray> ListModelProvenance::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[(int)ProvenanceRoles::NameRole] = "name";
	return roles;
}

QString ListModelProvenance::provenanceEverywhere() const
{
	return m_rConverter.getProvenanceEverywhere();
}

QString ListModelProvenance::name(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getSortOrdersCount() + 1)
		return "";

	if(row == c_uiRowEverywhere)
	{
		return provenanceEverywhere();
	}

	const SortOrder& rSortOrder = handle.data().getSortOrderAt(row - 1);
	return rSortOrder.getName();
}

int ListModelProvenance::renameProvenance(int row, QString newName)
{
	recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

	// Provenance "Everywhere" can't be renamed.
	if(row <= 0 || row >= (int) handle.data().getSortOrdersCount() + 1)
		return -1;

	if(handle.data().existsSortOrder(newName))
	{
		return -1;
	}

	qint32 newIndex = handle.data().getSortOrderIndex(newName) + 1;
	if(row != newIndex)
	{
		beginMoveRows(QModelIndex(), row, row, QModelIndex(), newIndex);
	}

	if(newIndex > row)
	{
		newIndex -= 1;
	}

	SortOrder& rSortOrder = handle.data().getSortOrderAt(row - 1);
	handle.data().renameSortOrder(rSortOrder, newName);

	if(row != newIndex)
	{
		endMoveRows();
	}

	dataChanged(index(newIndex), index(newIndex));

	emit provenanceRenamed(newIndex);

	return newIndex;
}

int ListModelProvenance::addProvenance(QString strSortOrder)
{
	recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

	if(handle.data().existsSortOrder(strSortOrder) || strSortOrder == provenanceEverywhere())
	{
		return -1;
	}

	qint32 index = handle.data().getSortOrderIndex(strSortOrder) + 1;

	beginInsertRows(QModelIndex(), index, index);
	handle.data().addSortOrder(strSortOrder);
	endInsertRows();

	return index;
}

bool ListModelProvenance::existsProvenance(QString strSortOrder) const
{
	if(strSortOrder == provenanceEverywhere())
	{
		return true;
	}

	recipebook::RBDataReadHandle handle(m_rRBDataHandler);
	return handle.data().existsSortOrder(strSortOrder);
}

QString ListModelProvenance::listUsedInIngredients(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	// Not implemented for provenance "Everywhere".
	if(row <= 0 || row >= (int) handle.data().getSortOrdersCount() + 1)
		return " -";

	const SortOrder& rSortOrder = handle.data().getSortOrderAt(row - 1);

	QList<Ingredient*> ingredients;
	if(!handle.data().isSortOrderInUse(rSortOrder, &ingredients))
	{
		return " -";
	}

	QString text;
	if(ingredients.size() > 0)
	{
		text = "<ul>";
		for(Ingredient* pIngredient : qAsConst(ingredients))
		{
			text += "<li>" + pIngredient->getName() + "</li>";
		}
		text += "</ul>";
	}
	else
	{
		text = " -";
	}

	return text;
}

bool ListModelProvenance::canProvenanceBeRemoved(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row == c_uiRowEverywhere || row < 0 || row >= (int) handle.data().getSortOrdersCount() + 1)
	{
		return false;
	}

	const SortOrder& rSortOrder = handle.data().getSortOrderAt(row - 1);
	return !handle.data().isSortOrderInUse(rSortOrder);
}

bool ListModelProvenance::removeProvenance(int row)
{
	recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getSortOrdersCount() + 1)
	{
		return false;
	}

	if(!canProvenanceBeRemoved(row))
	{
		return false;
	}

	beginRemoveRows(QModelIndex(), row, row);

	SortOrder& rSortOrder = handle.data().getSortOrderAt(row - 1);
	bool bSuccess = handle.data().removeSortOrder(rSortOrder);

	endRemoveRows();

	return bSuccess;
}

void ListModelProvenance::onDataReset()
{
	beginResetModel();
	endResetModel();
}
