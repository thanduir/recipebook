#include "ListModelRecipeBookConfigurations.h"
#include <data/RecipeBook.h>
#include <data/RecipeBookConfiguration.h>
#include <data/RBDataHandler.h>

using namespace recipebook::UI;

ListModelRecipeBookConfigurations::ListModelRecipeBookConfigurations(recipebook::RBDataHandler& rRBDataHandler)
:	m_rRBDataHandler(rRBDataHandler)
{
}

int ListModelRecipeBookConfigurations::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);
    return (int)handle.data().getConfigurationsCount();
}

QVariant ListModelRecipeBookConfigurations::data(const QModelIndex& index, int iRole) const
{
	RBConfigurationsRoles role = static_cast<RBConfigurationsRoles>(iRole);
	if(role == RBConfigurationsRoles::NameRole)
	{
		return name(index.row());
	}
	else if(role == RBConfigurationsRoles::TitleRole)
	{
		return title(index.row());
	}
	else if(role == RBConfigurationsRoles::SubtitleRole)
	{
		return subtitle(index.row());
	}
	else if(role == RBConfigurationsRoles::FontSizeRole)
	{
		return fontSize(index.row());
	}

	return QVariant();
}

QHash<int, QByteArray> ListModelRecipeBookConfigurations::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[(int)RBConfigurationsRoles::NameRole] = "name";
	roles[(int)RBConfigurationsRoles::TitleRole] = "title";
	roles[(int)RBConfigurationsRoles::SubtitleRole] = "subtitle";
	roles[(int)RBConfigurationsRoles::FontSizeRole] = "fontSize";
	return roles;
}

QString ListModelRecipeBookConfigurations::name(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getConfigurationsCount())
		return "";

    const RecipeBookConfiguration& rConfig = handle.data().getConfigurationAt((quint32)row);
	return rConfig.getName();
}

int ListModelRecipeBookConfigurations::renameConfiguration(int row, QString newName)
{
	qint32 newIndex = -1;
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getConfigurationsCount())
			return -1;

		if(handle.data().existsConfiguration(newName))
		{
			return -1;
		}

        newIndex = (qint32)handle.data().getConfigurationIndex(newName);
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

        RecipeBookConfiguration& rConfig = handle.data().getConfigurationAt((quint32)row);
		handle.data().renameConfiguration(rConfig, newName);
	}

	if(row != newIndex)
	{
		endMoveRows();
	}

	dataChanged(index(newIndex), index(newIndex));

	return newIndex;
}

QString ListModelRecipeBookConfigurations::title(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getConfigurationsCount())
		return "";

    const RecipeBookConfiguration& rConfig = handle.data().getConfigurationAt((quint32)row);
	return rConfig.getBookTitle();
}

QString ListModelRecipeBookConfigurations::subtitle(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getConfigurationsCount())
		return "";

    const RecipeBookConfiguration& rConfig = handle.data().getConfigurationAt((quint32)row);
	return rConfig.getBookSubtitle();
}

quint32 ListModelRecipeBookConfigurations::fontSize(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getConfigurationsCount())
		return 0;

    const RecipeBookConfiguration& rConfig = handle.data().getConfigurationAt((quint32)row);
	return rConfig.getFontSize();
}


void ListModelRecipeBookConfigurations::setDataChanged(int row, RBConfigurationsRoles role)
{
	QVector<int> rolesChanged;
	rolesChanged.append((int)role);
	dataChanged(index(row), index(row), rolesChanged);
}

void ListModelRecipeBookConfigurations::setTitle(int row, QString strTitle)
{
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getConfigurationsCount())
			return;

        RecipeBookConfiguration& rConfig = handle.data().getConfigurationAt((quint32)row);
		rConfig.setBookTitle(strTitle);
	}

	setDataChanged(row, RBConfigurationsRoles::TitleRole);
}

void ListModelRecipeBookConfigurations::setSubtitle(int row, QString strSubtitle)
{
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getConfigurationsCount())
			return;

        RecipeBookConfiguration& rConfig = handle.data().getConfigurationAt((quint32)row);
		rConfig.setBookSubtitle(strSubtitle);
	}

	setDataChanged(row, RBConfigurationsRoles::SubtitleRole);
}

void ListModelRecipeBookConfigurations::setFontSize(int row, quint32 fontSize)
{
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getConfigurationsCount())
			return;

        RecipeBookConfiguration& rConfig = handle.data().getConfigurationAt((quint32)row);
		rConfig.setFontSize(fontSize);
	}

	setDataChanged(row, RBConfigurationsRoles::FontSizeRole);
}

bool ListModelRecipeBookConfigurations::canConfigurationBeAdded() const
{
	return true;
}

int ListModelRecipeBookConfigurations::addConfiguration(QString strConfig)
{
	if(!canConfigurationBeAdded())
	{
		return -1;
	}

	qint32 index = -1;
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);

		if(handle.data().existsConfiguration(strConfig))
		{
			return -1;
		}

        index = (qint32)handle.data().getConfigurationIndex(strConfig);
	}

	beginInsertRows(QModelIndex(), index, index);
	
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);
		handle.data().addConfiguration(strConfig);
	}

	endInsertRows();

	return index;
}

bool ListModelRecipeBookConfigurations::existsConfiguration(QString strConfig) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);
	return handle.data().existsConfiguration(strConfig);
}

bool ListModelRecipeBookConfigurations::removeConfiguration(int row)
{
	beginRemoveRows(QModelIndex(), row, row);
	bool bSuccess = false;
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);
        const RecipeBookConfiguration& rConfig = handle.data().getConfigurationAt((quint32)row);
		bSuccess = handle.data().removeConfiguration(rConfig);
	}
	endRemoveRows();

	return bSuccess;
}

void ListModelRecipeBookConfigurations::onDataReset()
{
	beginResetModel();
	endResetModel();
}
