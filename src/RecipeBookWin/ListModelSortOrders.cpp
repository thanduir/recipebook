#include "ListModelSortOrders.h"
#include <data/RecipeBook.h>
#include <data/Ingredient.h>
#include <data/SortOrder.h>
#include <data/RBDataHandler.h>
#include "uistringconverter.h"

using namespace recipebook::UI;

ListModelSortOrders::ListModelSortOrders(recipebook::RBDataHandler& rRBDataHandler, const UIStringConverter& rConverter)
:	m_rRBDataHandler(rRBDataHandler),
	m_rConverter(rConverter)
{
}

int ListModelSortOrders::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);
	return handle.data().getSortOrdersCount();
}

QVariant ListModelSortOrders::data(const QModelIndex& index, int iRole) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if (index.row() < 0 || index.row() >= (int)handle.data().getIngredientsCount())
		return QVariant();

	const SortOrder& rSortOrder = handle.data().getSortOrderAt(index.row());
	SortOrdersRoles role = static_cast<SortOrdersRoles>(iRole);
	if(role == SortOrdersRoles::NameRole)
	{
		return rSortOrder.getName();
	}

	return QVariant();
}

QHash<int, QByteArray> ListModelSortOrders::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[(int)SortOrdersRoles::NameRole] = "name";
	return roles;
}

QString ListModelSortOrders::name(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getSortOrdersCount())
		return "";

	const SortOrder& rSortOrder = handle.data().getSortOrderAt(row);
	return rSortOrder.getName();
}

int ListModelSortOrders::renameSortOrder(int row, QString newName)
{
	qint32 newIndex = -1;
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getSortOrdersCount())
			return -1;

		if(handle.data().existsSortOrder(newName))
		{
			return -1;
		}

		newIndex = handle.data().getSortOrderIndex(newName);
	}

	if(row != newIndex)
	{
		beginMoveRows(QModelIndex(), row, row, QModelIndex(), newIndex);
	}

	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

		if(newIndex > row)
		{
			newIndex -= 1;
		}	

		SortOrder& rSortOrder = handle.data().getSortOrderAt(row);
		handle.data().renameSortOrder(rSortOrder, newName);
	}

	if(row != newIndex)
	{
		endMoveRows();
	}

	dataChanged(index(newIndex), index(newIndex));

	emit sortOrderRenamed(newIndex);

	return newIndex;
}

int ListModelSortOrders::addSortOrder(QString strSortOrder)
{
	qint32 index = -1;
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);

		if(handle.data().existsSortOrder(strSortOrder))
		{
			return -1;
		}

		index = handle.data().getSortOrderIndex(strSortOrder);
	}

	beginInsertRows(QModelIndex(), index, index);
	
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);
		handle.data().addSortOrder(strSortOrder);
	}

	endInsertRows();

	return index;
}

bool ListModelSortOrders::existsSortOrder(QString strSortOrder) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);
	return handle.data().existsSortOrder(strSortOrder);
}

bool ListModelSortOrders::removeSortOrder(int row)
{
	beginRemoveRows(QModelIndex(), row, row);
	bool bSuccess = false;
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);
		const SortOrder& rSortOrder = handle.data().getSortOrderAt(row);
		bSuccess = handle.data().removeSortOrder(rSortOrder);
	}
	endRemoveRows();

	return bSuccess;
}

void ListModelSortOrders::onDataReset()
{
	beginResetModel();
	endResetModel();
}
