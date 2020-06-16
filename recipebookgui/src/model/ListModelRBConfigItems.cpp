#include "ListModelRBConfigItems.h"
#include <QException>
#include <data/RecipeBook.h>
#include <data/RecipeBookConfiguration.h>
#include <data/RecipeBookConfigItem.h>
#include <data/RBDataHandler.h>

using namespace recipebook::UI;

ListModelRBConfigItems::ListModelRBConfigItems(recipebook::RBDataHandler& rRBDataHandler)
:	m_rRBDataHandler(rRBDataHandler)
{
}

void ListModelRBConfigItems::setCurrentConfig(int row)
{
	if(m_CurrentConfig == row)
	{
		return;
	}

	beginResetModel();
	m_CurrentConfig = row;
	endResetModel();
}

recipebook::RecipeBookConfiguration* ListModelRBConfigItems::getConfig(recipebook::RBDataWriteHandle& rHandle)
{
	if(m_CurrentConfig < 0 || m_CurrentConfig >= (int) rHandle.data().getConfigurationsCount())
	{
		return nullptr;
	}
    return &rHandle.data().getConfigurationAt((quint32)m_CurrentConfig);
}

const recipebook::RecipeBookConfiguration* ListModelRBConfigItems::getConfig(recipebook::RBDataReadHandle& rHandle) const
{
	if(m_CurrentConfig < 0 || m_CurrentConfig >= (int) rHandle.data().getConfigurationsCount())
	{
		return nullptr;
	}
    return &rHandle.data().getConfigurationAt((quint32)m_CurrentConfig);
}

int ListModelRBConfigItems::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);

	if(m_CurrentConfig < 0)
	{
		return 0;
	}

	RBDataReadHandle handle(m_rRBDataHandler);
	const RecipeBookConfiguration* pConfig = getConfig(handle);
	if(pConfig == nullptr)
		return 0;

    return (int)pConfig->getItemsCount();
}

QVariant ListModelRBConfigItems::data(const QModelIndex& index, int iRole) const
{
	if(m_CurrentConfig < 0)
	{
		return QVariant();
	}

	RBConfigItemsRoles role = static_cast<RBConfigItemsRoles>(iRole);
	if(role == RBConfigItemsRoles::NameRole)
	{
		return name(index.row());
	}
	else if(role == RBConfigItemsRoles::HeaderRole)
	{
		return isHeader(index.row());
	}
	else if(role == RBConfigItemsRoles::HeaderLevelRole)
	{
		return headerLevel(index.row());
	}
	else if (role == RBConfigItemsRoles::MaxHeaderLevelRole)
	{
		return maxHeaderLevel(index.row());
	}

	return QVariant();
}

bool ListModelRBConfigItems::setData(const QModelIndex& index, const QVariant& value, int iRole)
{
	if(m_CurrentConfig < 0)
	{
		return false;
	}

	RBConfigItemsRoles role = static_cast<RBConfigItemsRoles>(iRole);
	if(role == RBConfigItemsRoles::NameRole)
	{
		return renameHeader(index.row(), value.toString());
	}
	else if(role == RBConfigItemsRoles::HeaderRole)
	{
		return false;
	}
	else if(role == RBConfigItemsRoles::HeaderLevelRole)
	{
		setHeaderLevel(index.row(), value.toUInt());
		return true;
	}

	return false;
}

QHash<int, QByteArray> ListModelRBConfigItems::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[(int)RBConfigItemsRoles::NameRole] = "name";
	roles[(int)RBConfigItemsRoles::HeaderRole] = "isHeader";
	roles[(int)RBConfigItemsRoles::HeaderLevelRole] = "headerLevel";
	roles[(int)RBConfigItemsRoles::MaxHeaderLevelRole] = "maxHeaderLevel";
	return roles;
}

void ListModelRBConfigItems::setDataChanged(int row, RBConfigItemsRoles role)
{
	QVector<int> rolesChanged;
	rolesChanged.append((int)role);
	dataChanged(index(row), index(row), rolesChanged);
}

qint32 ListModelRBConfigItems::getPreviousHeaderLevel(const RecipeBookConfiguration& rConfig, int row) const
{
    if(row <= 0)
    {
        return 0;
    }

	// Find correct level (the same as the previous header)
    for (quint32 i = (quint32)row - 1; i >= 0; --i)
	{
		if (rConfig.getItemAt(i).getType() == RecipeBookConfigItemType::Header)
		{
            return (qint32)rConfig.getItemAt(i).getLevel();
		}
	}

	return 0;
}

QString ListModelRBConfigItems::name(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const RecipeBookConfiguration* pConfig = getConfig(handle);

	if(pConfig == nullptr || row < 0 || row >= (int)pConfig->getItemsCount())
		return "";

    const RecipeBookConfigItem& rItem = pConfig->getItemAt((quint32)row);
	return rItem.getName();
}

bool ListModelRBConfigItems::existsRecipe(QString strRecipe) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const RecipeBookConfiguration* pConfig = getConfig(handle);

	if(pConfig == nullptr || !handle.data().existsRecipe(strRecipe))
		return false;
	
	return pConfig->existsRecipe(handle.data().getRecipe(strRecipe));
}

bool ListModelRBConfigItems::isHeader(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const RecipeBookConfiguration* pConfig = getConfig(handle);

	if(pConfig == nullptr || row < 0 || row >= (int)pConfig->getItemsCount())
		return false;

    const RecipeBookConfigItem& rItem = pConfig->getItemAt((quint32)row);
	return rItem.getType() == RecipeBookConfigItemType::Header;
}

quint32 ListModelRBConfigItems::headerLevel(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const RecipeBookConfiguration* pConfig = getConfig(handle);

	if(pConfig == nullptr || row < 0 || row >= (int)pConfig->getItemsCount())
		return 0;

    const RecipeBookConfigItem& rItem = pConfig->getItemAt((quint32)row);
    return (quint32)rItem.getLevel();
}

quint32 ListModelRBConfigItems::maxHeaderLevel(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const RecipeBookConfiguration* pConfig = getConfig(handle);

	if (pConfig == nullptr || row < 0 || row >= (int)pConfig->getItemsCount())
		return 0;

    return (quint32)getPreviousHeaderLevel(*pConfig, row) + 1;
}

bool ListModelRBConfigItems::renameHeader(int row, QString newName)
{
	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		RecipeBookConfiguration* pConfig = getConfig(handle);

		if(pConfig == nullptr || row < 0 || row >= (int) pConfig->getItemsCount())
			return false;

        RecipeBookConfigItem& rItem = pConfig->getItemAt((quint32)row);
		if(rItem.getType() != RecipeBookConfigItemType::Header)
		{
			return false;
		}
		
		if(!rItem.rename(newName))
		{
			return false;
		}
	}

	setDataChanged(row, RBConfigItemsRoles::NameRole);
	return true;
}
        
void ListModelRBConfigItems::setHeaderLevel(int row, quint32 level)
{
	if (level > maxHeaderLevel(row))
	{
		return;
	}

	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		RecipeBookConfiguration* pConfig = getConfig(handle);

		if(pConfig == nullptr || row < 0 || row >= (int) pConfig->getItemsCount())
			return;

        RecipeBookConfigItem& rItem = pConfig->getItemAt((quint32)row);
        rItem.setLevel((qint32)level);
	}

	setDataChanged(row, RBConfigItemsRoles::HeaderLevelRole);
	setDataChanged(row, RBConfigItemsRoles::MaxHeaderLevelRole);
}

bool ListModelRBConfigItems::canRecipesBeAdded() const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const RecipeBookConfiguration* pConfig = getConfig(handle);
	if(pConfig == nullptr)
	{
		return false;
	}

	for(quint32 i = 0; i < handle.data().getRecipesCount(); ++i)
	{
		const Recipe& rRecipe = handle.data().getRecipeAt(i);
		if(!pConfig->existsRecipe(rRecipe))
		{
			return true;
		}
	}
	
	return false;
}

int ListModelRBConfigItems::addHeader(QString strName, int pos)
{
	quint32 uiLevel = 0;
	{
		RBDataReadHandle handle(m_rRBDataHandler);
		const RecipeBookConfiguration* pConfig = getConfig(handle);

		if(pConfig == nullptr)
		{
			return -1;
		}

		if(pos == -1)
		{
            pos = (int)pConfig->getItemsCount();
		}

		// Find the correct level (the same as the previous header)
        uiLevel = (quint32)getPreviousHeaderLevel(*pConfig, pos);
	}

	beginInsertRows(QModelIndex(), pos, pos);

	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		RecipeBookConfiguration* pConfig = getConfig(handle);
		pConfig->addHeader(strName, uiLevel, pos);
	}

	endInsertRows();

	return pos;
}

int ListModelRBConfigItems::addRecipe(QString strName, int pos)
{
	if(!canRecipesBeAdded())
	{
		return -1;
	}

	{
		RBDataReadHandle handle(m_rRBDataHandler);
		const RecipeBookConfiguration* pConfig = getConfig(handle);

		if(pConfig == nullptr || !handle.data().existsRecipe(strName))
		{
			return -1;
		}

		const Recipe& rRecipe = handle.data().getRecipe(strName);
		if(pConfig->existsRecipe(rRecipe))
		{
			return -1;
		}

		if(pos == -1)
		{
            pos = (int)pConfig->getItemsCount();
		}
	}

	beginInsertRows(QModelIndex(), pos, pos);

	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		RecipeBookConfiguration* pConfig = getConfig(handle);
		const Recipe& rRecipe = handle.data().getRecipe(strName);
		pConfig->addRecipe(rRecipe, pos);
	}

	endInsertRows();

	return pos;
}

bool ListModelRBConfigItems::removeItem(int row)
{
	bool bIsHeader = false;
	qint32 uiLevel = 0;

	bool bSuccess = false;
	{
		RBDataReadHandle handle(m_rRBDataHandler);
		const RecipeBookConfiguration* pConfig = getConfig(handle);

		if(pConfig == nullptr || row < 0 || row >= (int) pConfig->getItemsCount())
			return false;

        const RecipeBookConfigItem& rItem = pConfig->getItemAt((quint32)row);
		if (rItem.getType() == RecipeBookConfigItemType::Header)
		{
			bIsHeader = true;
			uiLevel = rItem.getLevel();
		}
		
	}

	beginRemoveRows(QModelIndex(), row, row);

	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		RecipeBookConfiguration* pConfig = getConfig(handle);
        bSuccess = pConfig->removeItem((quint32)row);
	}

	endRemoveRows();

	if(bIsHeader)
	{
		// Adjust levels of further headers if needed

		QList<int> changedLevels;

		{
			RBDataWriteHandle handle(m_rRBDataHandler);
			RecipeBookConfiguration* pConfig = getConfig(handle);

			if(row < (int)pConfig->getItemsCount() && uiLevel > getPreviousHeaderLevel(*pConfig, row))
			{
				for(quint32 i = (quint32)row; i < pConfig->getItemsCount(); ++i)
				{
					RecipeBookConfigItem& rItem = pConfig->getItemAt(i);
					if(rItem.getType() == RecipeBookConfigItemType::Header)
					{
						if(rItem.getLevel() <= uiLevel)
						{
							break;
						}

						rItem.setLevel(rItem.getLevel() - 1);
                        changedLevels.append((int)i);
					}
				}
			}
		}

		for (int i : qAsConst(changedLevels))
		{
			setDataChanged(i, RBConfigItemsRoles::HeaderLevelRole);
			setDataChanged(i, RBConfigItemsRoles::MaxHeaderLevelRole);
		}
	}

	return bSuccess;
}

void ListModelRBConfigItems::moveItem(int row, int target)
{
	{
		RBDataReadHandle handle(m_rRBDataHandler);
		const RecipeBookConfiguration* pConfig = getConfig(handle);

		if(pConfig == nullptr || row < 0 || row >= (int)pConfig->getItemsCount()
			|| target < 0 || target >= (int)pConfig->getItemsCount()
			|| row == target)
			return;
	}

	beginMoveRows(QModelIndex(), row, row, QModelIndex(), target > row ? target + 1 : target);

	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		RecipeBookConfiguration* pConfig = getConfig(handle);
        pConfig->moveItem(pConfig->getItemAt((quint32)row), (quint32)target);
	}

	endMoveRows();
}

void ListModelRBConfigItems::beginMove(int row)
{
	m_MoveFrom = row;
	m_MoveTo = row;
}

void ListModelRBConfigItems::updateMoveTarget(int target)
{
	m_MoveTo = target;
}

void ListModelRBConfigItems::applyMove()
{
	if(m_MoveFrom != m_MoveTo)
	{
		moveItem(m_MoveFrom, m_MoveTo);
	}

	m_MoveFrom = -1;
	m_MoveTo = -1;
}

void ListModelRBConfigItems::onDependentItemRemoved(int index)
{
	Q_UNUSED(index);

	beginResetModel();
	endResetModel();
}

void ListModelRBConfigItems::beginEditList()
{
	m_EditListSelectedValues.clear();
}

bool ListModelRBConfigItems::itemSelected(QString itemName) const
{
	return m_EditListSelectedValues.contains(itemName);
}

void ListModelRBConfigItems::changeState(QString itemName, bool selected)
{
	if(selected)
	{
		if(!m_EditListSelectedValues.contains(itemName))
		{
			m_EditListSelectedValues.append(itemName);
		}
	}
	else
	{
		m_EditListSelectedValues.removeOne(itemName);
	}
}

void ListModelRBConfigItems::cancelEditList()
{
	m_EditListSelectedValues.clear();
}

bool ListModelRBConfigItems::applyEditList()
{

	for (QString item : qAsConst(m_EditListSelectedValues))
	{
		addRecipe(item, -1);
	}

	bool bListChanged = m_EditListSelectedValues.size() > 0;

	m_EditListSelectedValues.clear();

	return bListChanged;
}
