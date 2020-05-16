#include "ListModelGoShopping.h"
#include <QException>
#include <data/RecipeBook.h>
#include <data/GoShoppingListItem.h>
#include <data/RBDataHandler.h>
#include "UIStringConverter.h"

using namespace recipebook::UI;

ListModelGoShopping::ListModelGoShopping(recipebook::RBDataHandler& rRBDataHandler, const UIStringConverter& rConverter)
:	m_rRBDataHandler(rRBDataHandler),
	m_rConverter(rConverter)
{
}

void ListModelGoShopping::updateList()
{
	recipebook::RBDataWriteHandle handle(m_rRBDataHandler);
	m_List.updateList(handle.data());
}

void ListModelGoShopping::setSortOrder(QString strSortOrder, bool bSeparateCollectedItems)
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);
	const RecipeBook& rRecipeBook = handle.data();

	if(!rRecipeBook.existsSortOrder(strSortOrder))
	{
		return;
	}

	SortedShoppingListOrdering ordering = bSeparateCollectedItems ? SortedShoppingListOrdering::SeparateTakenItems : SortedShoppingListOrdering::Combined;

	beginResetModel();
	m_List.changeSortOrder(rRecipeBook.getSortOrder(strSortOrder), ordering);
	endResetModel();
}

int ListModelGoShopping::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);

	return m_List.getItemsCount();
}

QVariant ListModelGoShopping::data(const QModelIndex& index, int iRole) const
{
	GoShoppingRoles role = static_cast<GoShoppingRoles>(iRole);
	if(role == GoShoppingRoles::NameRole)
	{
		return name(index.row());
	}
	else if(role == GoShoppingRoles::TopLevelHeaderRole)
	{
		return isTopLevelHeader(index.row());
	}
	else if(role == GoShoppingRoles::CategoryHeaderRole)
	{
		return isCategoryHeader(index.row());
	}
	else if(role == GoShoppingRoles::IncompatibleItemsHeaderRole)
	{
		return isIncompatibleItemsHeader(index.row());
	}
	else if(role == GoShoppingRoles::NormalItemRole)
	{
		return isNormalItem(index.row());
	}
	else if(role == GoShoppingRoles::ItemCheckedRole)
	{
		return isItemChecked(index.row());
	}

	return QVariant();
}

QHash<int, QByteArray> ListModelGoShopping::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[(int)GoShoppingRoles::NameRole] = "name";
	roles[(int)GoShoppingRoles::TopLevelHeaderRole] = "topLevelHeader";
	roles[(int)GoShoppingRoles::CategoryHeaderRole] = "categoryHeader";
	roles[(int)GoShoppingRoles::IncompatibleItemsHeaderRole] = "incompatiblesHeader";
	roles[(int)GoShoppingRoles::NormalItemRole] = "normalItem";
	roles[(int)GoShoppingRoles::ItemCheckedRole] = "itemChecked";
	
	return roles;
}

bool ListModelGoShopping::setData(const QModelIndex& index, const QVariant& value, int iRole)
{
	GoShoppingRoles role = static_cast<GoShoppingRoles>(iRole);
	if(role == GoShoppingRoles::ItemCheckedRole)
	{
		setItemChecked(index.row(), value.toBool());
		return true;
	}

	return false;
}

void ListModelGoShopping::setDataChanged(int row, GoShoppingRoles role)
{
	QVector<int> rolesChanged;
	rolesChanged.append((int)role);
	dataChanged(index(row), index(row), rolesChanged);
}

QString ListModelGoShopping::name(int row) const
{
	if(row < 0 || row >= (int) m_List.getItemsCount())
		return "";

	const GoShoppingListItem& rItem = m_List.getItemAt(row);
	if(rItem.getType() == GoShoppingListItemType::Toplevel_Header_CheckedItems)
	{
		return tr("Collected items");
	}
	else if(rItem.getType() == GoShoppingListItemType::Toplevel_Header_UncheckedItems)
	{
		return tr("Uncollected items");
	}
	else
	{
		return m_List.getItemAt(row).getName();
	}
	
}

bool ListModelGoShopping::isItemChecked(int row) const
{
	if(row < 0 || row >= (int) m_List.getItemsCount())
		return false;

	return m_List.getItemAt(row).getStatus() == Status::Taken;
}

recipebook::GoShoppingListItemType ListModelGoShopping::itemType(int row) const
{
	if(row < 0 || row >= (int) m_List.getItemsCount())
		return GoShoppingListItemType::IngredientListItem;

	return m_List.getItemAt(row).getType();
}

bool ListModelGoShopping::isTopLevelHeader(int row) const
{
	GoShoppingListItemType eType = itemType(row);
	return eType == GoShoppingListItemType::Toplevel_Header_CheckedItems
		   || eType == GoShoppingListItemType::Toplevel_Header_UncheckedItems;
}

bool ListModelGoShopping::isCategoryHeader(int row) const
{
	return itemType(row) == GoShoppingListItemType::Category_Header;
}

bool ListModelGoShopping::isIncompatibleItemsHeader(int row) const
{
	return itemType(row) == GoShoppingListItemType::Incompatible_Items_Header;
}

bool ListModelGoShopping::isNormalItem(int row) const
{
	return itemType(row) == GoShoppingListItemType::IngredientListItem;
}

void ListModelGoShopping::setItemChecked(int row, bool bChecked)
{
	if(row < 0 || row >= (int) m_List.getItemsCount())
		return;

	Status status = bChecked ? Status::Taken : Status::None;
	m_List.updateStatus(m_rRBDataHandler, row, status, this);
}

void ListModelGoShopping::onDataReset()
{
	beginResetModel();
	m_List.clearList();
	endResetModel();
}

void ListModelGoShopping::itemChanged(int posItem)
{ 
	setDataChanged(posItem, GoShoppingRoles::ItemCheckedRole);
}

void ListModelGoShopping::beginMoveItem(int currentItemPos, int newItemPos)
{
	beginMoveRows(QModelIndex(), currentItemPos, currentItemPos, QModelIndex(), newItemPos);
}

void ListModelGoShopping::endMoveItem()
{
	endMoveRows();
}

void ListModelGoShopping::beginInsertItem(int posItem)
{
	beginInsertRows(QModelIndex(), posItem, posItem);
}

void ListModelGoShopping::endInsertItem()
{
	endInsertRows();
}

void ListModelGoShopping::beginRemoveItem(int posItem)
{
	beginRemoveRows(QModelIndex(), posItem, posItem);
}

void ListModelGoShopping::endRemoveItem()
{
	endRemoveRows();
}