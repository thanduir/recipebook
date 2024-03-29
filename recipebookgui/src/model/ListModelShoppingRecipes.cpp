#include "ListModelShoppingRecipes.h"
#include <QException>
#include <data/RecipeBook.h>
#include <data/ShoppingRecipe.h>
#include <data/ShoppingListItem.h>
#include <data/AlternativesType.h>
#include <data/RBDataHandler.h>
#include "../RecipeBookSettings.h"

using namespace recipebook::UI;

ListModelShoppingRecipes::ListModelShoppingRecipes(recipebook::RBDataHandler& rRBDataHandler, 
												   const UIStringConverter& rConverter, 
												   const recipebook::RecipeBookSettings& rSettings)
:	m_rRBDataHandler(rRBDataHandler),
	m_rConverter(rConverter),
	m_rSettings(rSettings)
{
}

int ListModelShoppingRecipes::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);
    return (int)handle.data().getShoppingRecipesCount();
}

QVariant ListModelShoppingRecipes::data(const QModelIndex& index, int iRole) const
{
	ShoppingRecipeRoles role = static_cast<ShoppingRecipeRoles>(iRole);
	if(role == ShoppingRecipeRoles::NameRole)
	{
		return name(index.row());
	}
	else if(role == ShoppingRecipeRoles::ScalingFactorRole)
	{
		return scalingFactor(index.row());
	}
	else if(role == ShoppingRecipeRoles::DueDateRole)
	{
		return dueDate(index.row());
	}
	else if(role == ShoppingRecipeRoles::DueDateSetRole)
	{
		return isDueDateSet(index.row());
	}
	else if(role == ShoppingRecipeRoles::EverythingSetRole)
	{
		return everythingSet(index.row());
	}

	return QVariant();
}

QHash<int, QByteArray> ListModelShoppingRecipes::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[(int)ShoppingRecipeRoles::NameRole] = "name";
	roles[(int)ShoppingRecipeRoles::ScalingFactorRole] = "scalingFactor";
	roles[(int)ShoppingRecipeRoles::DueDateRole] = "dueDate";
	roles[(int)ShoppingRecipeRoles::DueDateSetRole] = "dueDateSet";
	roles[(int)ShoppingRecipeRoles::EverythingSetRole] = "everythingSet";
	return roles;
}

void ListModelShoppingRecipes::setDataChanged(int row, ShoppingRecipeRoles role)
{
	QVector<int> rolesChanged;
	rolesChanged.append((int)role);
	dataChanged(index(row), index(row), rolesChanged);
}

QString ListModelShoppingRecipes::name(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getShoppingRecipesCount())
		return "";

    const ShoppingRecipe& rRecipe = handle.data().getShoppingRecipeAt((quint32)row);
	return rRecipe.getName();
}

float ListModelShoppingRecipes::scalingFactor(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getShoppingRecipesCount())
		return 0;

    const ShoppingRecipe& rRecipe = handle.data().getShoppingRecipeAt((quint32)row);
	return rRecipe.getScalingFactor();
}

QDate ListModelShoppingRecipes::dueDate(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getShoppingRecipesCount())
		return QDate();

    const ShoppingRecipe& rRecipe = handle.data().getShoppingRecipeAt((quint32)row);
	return rRecipe.getDueDate();
}

bool ListModelShoppingRecipes::isDueDateSet(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getShoppingRecipesCount())
		return false;

    const ShoppingRecipe& rRecipe = handle.data().getShoppingRecipeAt((quint32)row);
	return rRecipe.getDueDate().isValid();
}

bool ListModelShoppingRecipes::everythingSet(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getShoppingRecipesCount())
		return true;

    const ShoppingRecipe& rRecipe = handle.data().getShoppingRecipeAt((quint32)row);

	QMap<QString, bool> mapGroups;
	for(quint32 i = 0; i < rRecipe.getItemsCount(); ++i)
	{
		const ShoppingListItem& rItem = rRecipe.getItemAt(i);
		if(rItem.hasAlternativesGroup())
		{
			QString group = rItem.getAlternativesGroup().getElementId().toString();
			mapGroups[group] = mapGroups.value(group, false) || rItem.isItemEnabled();
		}
	}

	for(bool bValue : mapGroups.values())
	{
		if(!bValue)
		{
			return false;
		}
	}
	return true;
}

void ListModelShoppingRecipes::setScalingFactor(int row, float fFactor)
{
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getShoppingRecipesCount())
			return;

        ShoppingRecipe& rRecipe = handle.data().getShoppingRecipeAt((quint32)row);
		rRecipe.changeScalingFactor(fFactor);
	}

	setDataChanged(row, ShoppingRecipeRoles::ScalingFactorRole);
    emit recipeScalingChanged((quint32)row);
}

void ListModelShoppingRecipes::setDueDate(int row, QDate date)
{
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getShoppingRecipesCount())
			return;

        ShoppingRecipe& rRecipe = handle.data().getShoppingRecipeAt((quint32)row);
		rRecipe.setDueDate(date);
	}

	setDataChanged(row, ShoppingRecipeRoles::DueDateSetRole);
	setDataChanged(row, ShoppingRecipeRoles::DueDateRole);
}

void ListModelShoppingRecipes::resetDueDate(int row)
{
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getShoppingRecipesCount())
			return;

        ShoppingRecipe& rRecipe = handle.data().getShoppingRecipeAt((quint32)row);
		rRecipe.setDueDate(QDate());
	}

	setDataChanged(row, ShoppingRecipeRoles::DueDateSetRole);
	setDataChanged(row, ShoppingRecipeRoles::DueDateRole);
}

int ListModelShoppingRecipes::renameRecipe(int row, QString newName)
{
	qint32 newIndex = -1;
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getShoppingRecipesCount())
			return -1;

		if(handle.data().existsShoppingRecipe(newName))
		{
			return -1;
		}

        newIndex = (qint32)handle.data().getShoppingRecipeIndex(newName);
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

        ShoppingRecipe& rRecipe = handle.data().getShoppingRecipeAt((quint32)row);
		handle.data().renameShoppingRecipe(rRecipe, newName);
	}

	if(row != newIndex)
	{
		endMoveRows();
	}

	dataChanged(index(newIndex), index(newIndex));

	return newIndex;
}

bool ListModelShoppingRecipes::canShoppingRecipesBeAddedFromRecipes() const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);
	return handle.data().getRecipesCount() > 0;
}

int ListModelShoppingRecipes::addFromRecipe(QString strRecipe)
{
	if(!canShoppingRecipesBeAddedFromRecipes())
	{
		return -1;
	}

	qint32 index = -1;
	QString strShoppingRecipe = strRecipe;
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);

		if(!handle.data().existsRecipe(strRecipe))
		{
			return -1;
		}

		int iNr = 2;
        while(handle.data().existsShoppingRecipe(strShoppingRecipe))
        {
			strShoppingRecipe = QString("%1 (%2)").arg(strRecipe).arg(iNr);
            ++iNr;
        }

        index = (qint32)handle.data().getShoppingRecipeIndex(strShoppingRecipe);
	}

	beginInsertRows(QModelIndex(), index, index);

	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);
		const Recipe& rRecipe = handle.data().getRecipe(strRecipe);
		handle.data().addShoppingRecipe(strShoppingRecipe, rRecipe);
	}

	endInsertRows();

	return index;
}

bool ListModelShoppingRecipes::canNewShoppingRecipesBeAdded() const
{
	return true;
}

int ListModelShoppingRecipes::addNewRecipe(QString strRecipe)
{
	if(!canNewShoppingRecipesBeAdded())
	{
		return -1;
	}

	qint32 index = -1;
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);

		if(handle.data().existsShoppingRecipe(strRecipe))
		{
			return -1;
		}

        index = (qint32)handle.data().getShoppingRecipeIndex(strRecipe);
	}

	beginInsertRows(QModelIndex(), index, index);

	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);
        handle.data().addNewShoppingRecipe(strRecipe, static_cast<float>(m_rSettings.getDefaultRecipeNrPersons()));
	}

	endInsertRows();

	return index;
}

bool ListModelShoppingRecipes::existsRecipe(QString strRecipe) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);
	return handle.data().existsShoppingRecipe(strRecipe);
}

bool ListModelShoppingRecipes::removeRecipe(int row)
{
	bool bSuccess = false;
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getShoppingRecipesCount())
		{
			return false;
		}
	}

	beginRemoveRows(QModelIndex(), row, row);

	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);
        ShoppingRecipe& rRecipe = handle.data().getShoppingRecipeAt((quint32)row);
		bSuccess = handle.data().removeShoppingRecipe(rRecipe);
	}

	endRemoveRows();

	return bSuccess;
}

ListModelShoppingListItems* ListModelShoppingRecipes::getItemsModel(int row)
{
	ListModelShoppingListItems* items = new ListModelShoppingListItems(m_rRBDataHandler, m_rConverter);
	items->setShoppingRecipe(row);

	connect(items, SIGNAL(shoppingitemEnabledChanged(int)),
			this, SLOT(onItemEnabledChanged(int)));

	connect(this, SIGNAL(recipeScalingChanged(quint32)),
			items, SLOT(onDependentItemChanged(quint32)));

	return items;
}

void ListModelShoppingRecipes::resetShoppingList()
{
	beginResetModel();
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);
		handle.data().clearShoppingList();
	}
	endResetModel();
}

void ListModelShoppingRecipes::onDataReset()
{
	beginResetModel();
	endResetModel();
}

void ListModelShoppingRecipes::beginAddList()
{
	m_AddListValues.clear();
}

quint32 ListModelShoppingRecipes::getInsertionsCount(QString itemName) const
{
	if(!m_AddListValues.contains(itemName))
	{
		return 0;
	}

	return m_AddListValues[itemName];
}

void ListModelShoppingRecipes::changeState(QString itemName, quint32 insertionsCount)
{
	if(insertionsCount > 0)
	{
		m_AddListValues[itemName] = insertionsCount;
	}
	else
	{
		m_AddListValues.remove(itemName);
	}
}

void ListModelShoppingRecipes::cancelAddList()
{
	m_AddListValues.clear();
}

bool ListModelShoppingRecipes::applyAddList()
{
	bool bSuccess = false;
	for(QString recipe : m_AddListValues.keys())
	{
		for(quint32 i = 0; i < m_AddListValues[recipe]; ++i)
		{
			int retval = addFromRecipe(recipe);
			if(retval != -1)
			{
				bSuccess = true;
			}
		}
	}

	bool bListChanged = bSuccess && m_AddListValues.size() > 0;
	m_AddListValues.clear();
	return bListChanged;
}

void ListModelShoppingRecipes::onItemEnabledChanged(int row)
{
	setDataChanged(row, ShoppingRecipeRoles::EverythingSetRole);
}
