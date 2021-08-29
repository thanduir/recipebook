#include "ListModelGoShopping.h"
#include <QException>
#include <data/RecipeBook.h>
#include <data/GoShoppingListItem.h>
#include <data/RBDataHandler.h>
#include "../uistringconverter.h"

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

    return (int)m_List.getItemsCount();
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
	else if(role == GoShoppingRoles::ItemHeaderRole)
	{
		return itemHeader(index.row());
	}
	else if(role == GoShoppingRoles::ItemMultilineRole)
	{
		return isItemMultline(index.row());
	}
	else if(role == GoShoppingRoles::ItemAdditionalTextRole)
	{
		return itemAdditionalText(index.row());
	}
	else if(role == GoShoppingRoles::ItemOptionalRole)
	{
		return isItemOptional(index.row());
	}
	else if(role == GoShoppingRoles::ItemCheckedRole)
	{
		return isItemChecked(index.row());
	}
	else if(role == GoShoppingRoles::ItemRecipeInfoRole)
	{
		return getRecipeInfo(index.row());
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
	roles[(int)GoShoppingRoles::ItemHeaderRole] = "itemHeader";
	roles[(int)GoShoppingRoles::ItemMultilineRole] = "itemMultiline";
	roles[(int)GoShoppingRoles::ItemAdditionalTextRole] = "itemAdditionalText";
	roles[(int)GoShoppingRoles::ItemOptionalRole] = "itemOptional";
	roles[(int)GoShoppingRoles::ItemCheckedRole] = "itemChecked";
	roles[(int)GoShoppingRoles::ItemRecipeInfoRole] = "itemRecipeInfo";	
	
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

    const GoShoppingListItem& rItem = m_List.getItemAt((quint32)row);
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
        return m_List.getItemAt((quint32)row).getName();
	}
}

bool ListModelGoShopping::isItemChecked(int row) const
{
	if(row < 0 || row >= (int) m_List.getItemsCount())
		return false;

    return m_List.getItemAt((quint32)row).getStatus() == Status::Taken;
}

QString ListModelGoShopping::itemHeader(int row) const
{
	if(row < 0 || row >= (int) m_List.getItemsCount())
		return "";

    const GoShoppingListItem& rItem = m_List.getItemAt((quint32)row);
	if(rItem.getType() != GoShoppingListItemType::IngredientListItem)
	{
		return "";
	}

	if(rItem.getAmount().size() == 1)
	{
		return m_rConverter.formatAmount(rItem.getAmount().at(0)) + " " + rItem.getName();
	}
	else if(rItem.getAmount().size() > 1)
	{
		QString strText;
		for(Amount amount : rItem.getAmount())
		{
			if(!strText.isEmpty())
			{
				strText += " + ";
			}
			strText += m_rConverter.formatAmount(amount, false);
		}
		return strText + " " + rItem.getName();
	}

	return "";
}

bool ListModelGoShopping::isItemMultline(int row) const
{
	if(row < 0 || row >= (int) m_List.getItemsCount())
		return false;

    const GoShoppingListItem& rItem = m_List.getItemAt((quint32)row);
	if(rItem.getType() != GoShoppingListItemType::IngredientListItem)
	{
		return false;
	}

	return rItem.getCombinedItemsCount() > 1;
}

QString ListModelGoShopping::itemAdditionalText(int row) const
{
	if(row < 0 || row >= (int) m_List.getItemsCount())
		return "";

    const GoShoppingListItem& rItem = m_List.getItemAt((quint32)row);
	if(rItem.getType() != GoShoppingListItemType::IngredientListItem)
	{
		return "";
	}

	if(rItem.getCombinedItemsCount() == 1)
	{
		QString addText;
		if(rItem.getSize(0) != Size::Normal)
		{
			addText += m_rConverter.convertSize(rItem.getSize(0), rItem.getAmount(0).getUnit());
		}
		if(!rItem.getAdditionalInfo(0).isEmpty())
		{
			if(!addText.isEmpty())
			{
				addText += ", ";
			}
			addText += rItem.getAdditionalInfo(0);
		}
		if(!addText.isEmpty())
		{
			addText = "(" + addText + ")";
		}

		return addText;
	}
	else if(rItem.getCombinedItemsCount() > 1)
	{
		QString lines;
		for(quint32 i = 0; i < rItem.getCombinedItemsCount(); ++i)
		{
			QString strText;
			if(rItem.getSize(i) != Size::Normal)
			{
				if(!strText.isEmpty())
				{
					strText += ", ";
				}
				strText += m_rConverter.convertSize(rItem.getSize(i), rItem.getAmount(i).getUnit());
			}

			if(rItem.isOptional(i))
			{
				if(!strText.isEmpty())
				{
					strText += ", ";
				}
				strText += tr("Optional");
			}

			if(!rItem.getAdditionalInfo(i).isEmpty())
			{
				if(!strText.isEmpty())
				{
					strText += ", ";
				}
				strText += rItem.getAdditionalInfo(i);
			}

			if(!strText.isEmpty())
			{
				QString strAmount = m_rConverter.formatAmount(rItem.getAmount(i));
				lines += QString(tr("<li>%1 of those %2</li>")).arg(strAmount).arg(strText);
			}
			else if(rItem.getAmount(i).getUnit() == Unit::Unitless)
			{
				lines += QString(tr("<li>Unitless amount for \"%1\"</li>")).arg(rItem.getRecipeInfos()[i].m_RecipeName);
			}
				 
		}

		if(!lines.isEmpty())
		{
			lines = "<ul>" + lines + "</ul>";
		}
		return lines;
	}
	
	return "";
}

bool ListModelGoShopping::isItemOptional(int row) const
{
	if(row < 0 || row >= (int) m_List.getItemsCount())
		return false;

    const GoShoppingListItem& rItem = m_List.getItemAt((quint32)row);
	if(rItem.getType() != GoShoppingListItemType::IngredientListItem)
	{
		return false;
	}

	return rItem.isOptional();
}

QString ListModelGoShopping::getRecipeInfo(int row) const
{
	if(row < 0 || row >= (int) m_List.getItemsCount())
		return "";

    const GoShoppingListItem& rItem = m_List.getItemAt((quint32)row);
	if(rItem.getType() != GoShoppingListItemType::IngredientListItem)
	{
		return "";
	}

	QString strText;
	const auto& rInfos = rItem.getRecipeInfos();
	bool bAsList = rInfos.size() > 1;
	for(const auto& rInfo : rInfos)
	{
		if(bAsList)
		{
			strText += "<li>";
		}

		QString amountText = m_rConverter.formatAmount(rInfo.m_Amount);
		if(amountText.isEmpty() || !bAsList)
		{
			strText += QString(tr("Recipe: \"%1\".")).arg(rInfo.m_RecipeName);
		}
		else
		{
			strText += QString(tr("%1 for recipe \"%2\".")).arg(amountText).arg(rInfo.m_RecipeName);
		}
		if(rInfo.m_DueDate.isValid())
		{
			if(bAsList)
			{
				strText += QString(tr(" Due date: %1.")).arg(rInfo.m_DueDate.toString("dddd, dd.MM.yyyy"));
			}
			else
			{
				strText += QString(tr("<br>Due date: %1.")).arg(rInfo.m_DueDate.toString("dddd, dd.MM.yyyy"));
			}
		}

		if(bAsList)
		{
			strText += "</li>";
		}
	}

	if(bAsList)
	{
		strText = "<ul>" + strText + "</ul>";
	}

	return strText;
}

recipebook::GoShoppingListItemType ListModelGoShopping::itemType(int row) const
{
	if(row < 0 || row >= (int) m_List.getItemsCount())
		return GoShoppingListItemType::IngredientListItem;

    return m_List.getItemAt((quint32)row).getType();
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
    m_List.updateStatus(m_rRBDataHandler, (quint32)row, status, this);
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
