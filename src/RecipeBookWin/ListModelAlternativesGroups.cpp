#include "ListModelAlternativesGroups.h"
#include <data/RecipeBook.h>
#include <data/AlternativesType.h>
#include <data/RBDataHandler.h>
#include "uistringconverter.h"

using namespace recipebook::UI;

const quint32 c_uiRowNoGroup = 0;

ListModelAlternativesGroups::ListModelAlternativesGroups(recipebook::RBDataHandler& rRBDataHandler, const UIStringConverter& rConverter)
:	m_rRBDataHandler(rRBDataHandler),
	m_rConverter(rConverter)
{
}

QString ListModelAlternativesGroups::stringNoAlternativesGroup() const
{
	return m_rConverter.getStringNoAlternativesGroup();
}

int ListModelAlternativesGroups::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);
	return handle.data().getAlternativesTypesCount() + 1;
}

QVariant ListModelAlternativesGroups::data(const QModelIndex& index, int iRole) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if (index.row() < 0 || index.row() >= (int)handle.data().getAlternativesTypesCount() + 1)
		return QVariant();

	AlternativesGroupsRoles role = static_cast<AlternativesGroupsRoles>(iRole);
	if(role == AlternativesGroupsRoles::NameRole)
	{
		return name(index.row());
	}
	else if(role == AlternativesGroupsRoles::ColorRole)
	{
		return color(index.row());
	}

	return QVariant();
}

QHash<int, QByteArray> ListModelAlternativesGroups::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[(int)AlternativesGroupsRoles::NameRole] = "name";
	roles[(int)AlternativesGroupsRoles::ColorRole] = "typeColor";
	return roles;
}

QString ListModelAlternativesGroups::name(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int)handle.data().getAlternativesTypesCount() + 1)
		return "";

	if(row == c_uiRowNoGroup)
	{
		return stringNoAlternativesGroup();
	}

	const AlternativesType& rType = handle.data().getAlternativesTypeAt(row - 1);
	return rType.getName();
}

int ListModelAlternativesGroups::renameType(int row, QString newName)
{
	qint32 newIndex = -1;
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);

		if(row == c_uiRowNoGroup || row < 0 || row >= (int) handle.data().getAlternativesTypesCount() + 1)
			return -1;

		if(handle.data().existsAlternativesType(newName))
		{
			return -1;
		}

		newIndex = handle.data().getAlternativesTypeIndex(newName);
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

		AlternativesType& rType = handle.data().getAlternativesTypeAt(row - 1);
		handle.data().renameAlternativesType(rType, newName);
	}

	if(row != newIndex)
	{
		endMoveRows();
	}

	dataChanged(index(newIndex), index(newIndex));

	emit alternativesGroupChanged(row);

	return newIndex;
}

QString ListModelAlternativesGroups::color(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row == c_uiRowNoGroup || row < 0 || row >= (int)handle.data().getAlternativesTypesCount() + 1)
		return "";

	const AlternativesType& rType = handle.data().getAlternativesTypeAt(row - 1);
	return rType.getColor().name();
}

void ListModelAlternativesGroups::setColor(int row, QString strColor)
{
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getIngredientsCount())
			return;

		AlternativesType& rType = handle.data().getAlternativesTypeAt(row - 1);
		rType.setColor(strColor);
	}

	QVector<int> rolesChanged;
	rolesChanged.append((int)AlternativesGroupsRoles::ColorRole);
	dataChanged(index(row), index(row), rolesChanged);

	emit alternativesGroupChanged(row);
}

bool ListModelAlternativesGroups::canTypesBeAdded() const
{
	return true;
}

int ListModelAlternativesGroups::addType(QString strType)
{
	if(!canTypesBeAdded())
	{
		return -1;
	}

	qint32 index = -1;
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);

		if(handle.data().existsAlternativesType(strType) || strType == stringNoAlternativesGroup())
		{
			return -1;
		}

		index = handle.data().getAlternativesTypeIndex(strType) + 1;
	}
	
	beginInsertRows(QModelIndex(), index, index);

	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);
		handle.data().addAlternativesType(strType);
	}

	endInsertRows();

	return index;
}

bool ListModelAlternativesGroups::existsType(QString strType) const
{
	if(strType == stringNoAlternativesGroup())
	{
		return true;
	}

	recipebook::RBDataReadHandle handle(m_rRBDataHandler);
	return handle.data().existsAlternativesType(strType);
}

bool ListModelAlternativesGroups::canTypeBeRemoved(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row == c_uiRowNoGroup || row < 0 || row >= (int)handle.data().getAlternativesTypesCount() + 1)
	{
		return false;
	}

	const AlternativesType& rType = handle.data().getAlternativesTypeAt(row - 1);
	return !handle.data().isAlternativesTypeInUse(rType);
}

bool ListModelAlternativesGroups::removeType(int row)
{
	bool bSuccess = false;
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);

		if(row == c_uiRowNoGroup || row < 0 || row >= (int) handle.data().getAlternativesTypesCount() + 1)
		{
			return false;
		}

		if(!canTypeBeRemoved(row))
		{
			return false;
		}
	}
	
	beginRemoveRows(QModelIndex(), row, row);

	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);
		const AlternativesType& rType = handle.data().getAlternativesTypeAt(row - 1);
		bSuccess = handle.data().removeAlternativesType(rType);
	}

	endRemoveRows();

	return bSuccess;
}

int ListModelAlternativesGroups::indexOf(QString strName) const
{
	if(strName == m_rConverter.getStringNoAlternativesGroup())
	{
		return 0;
	}

	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(!handle.data().existsAlternativesType(strName))
	{
		return -1;
	}

	return handle.data().getAlternativesTypeIndex(strName) + 1;
}

void ListModelAlternativesGroups::onDataReset()
{
	beginResetModel();
	endResetModel();
}
