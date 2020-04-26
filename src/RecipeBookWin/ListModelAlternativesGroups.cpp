#include "ListModelAlternativesGroups.h"
#include <data/RecipeBook.h>
#include <data/AlternativesType.h>
#include "uistringconverter.h"
#include "RBDataHandler.h"

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

	if(index.row() == c_uiRowNoGroup)
	{
		return stringNoAlternativesGroup();
	}

	const AlternativesType& rType = handle.data().getAlternativesTypeAt(index.row() - 1);
	AlternativesGroupsRoles role = static_cast<AlternativesGroupsRoles>(iRole);
	if(role == AlternativesGroupsRoles::NameRole)
	{
		return rType.getName();
	}
	else if(role == AlternativesGroupsRoles::ColorRole)
	{
		return rType.getColor();
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
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

		if(row == c_uiRowNoGroup || row < 0 || row >= (int) handle.data().getAlternativesTypesCount() + 1)
			return -1;

		if(handle.data().existsAlternativesType(newName))
		{
			return -1;
		}

		AlternativesType& rType = handle.data().getAlternativesTypeAt(row - 1);
		handle.data().renameAlternativesType(rType, newName);
	}

	QVector<int> rolesChanged;
	rolesChanged.append((int)AlternativesGroupsRoles::NameRole);
	dataChanged(index(row), index(row), rolesChanged);
    
	emit alternativesGroupChanged(row);

	return row;
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

int ListModelAlternativesGroups::addType(QString strType)
{
	qint32 index = -1;
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

		if(handle.data().existsAlternativesType(strType) || strType == stringNoAlternativesGroup())
		{
			return -1;
		}

		index = handle.data().getAlternativesTypeIndex(strType) + 1;

		beginInsertRows(QModelIndex(), index, index);
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
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

		if(row == c_uiRowNoGroup || row < 0 || row >= (int) handle.data().getAlternativesTypesCount() + 1)
		{
			return false;
		}

		if(!canTypeBeRemoved(row))
		{
			return false;
		}

		beginRemoveRows(QModelIndex(), row, row);

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
