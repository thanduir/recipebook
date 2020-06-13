#include "ListModelShoppingListItems.h"
#include <QException>
#include <data/RecipeBook.h>
#include <data/ShoppingRecipe.h>
#include <data/ShoppingListItem.h>
#include <data/AlternativesType.h>
#include <data/RBDataHandler.h>
#include "../uistringconverter.h"

using namespace recipebook::UI;

ListModelShoppingListItems::ListModelShoppingListItems(recipebook::RBDataHandler& rRBDataHandler, const UIStringConverter& rConverter)
:	m_rRBDataHandler(rRBDataHandler),
	m_rConverter(rConverter),
	m_ShoppingRecipeIndex(-1)
{
}

void ListModelShoppingListItems::setShoppingRecipe(int row)
{
	if(m_ShoppingRecipeIndex == row)
	{
		return;
	}

	beginResetModel();
	m_ShoppingRecipeIndex = row;
	endResetModel();
}

recipebook::ShoppingRecipe* ListModelShoppingListItems::getShoppingRecipe(recipebook::RBDataWriteHandle& rHandle)
{
	if(m_ShoppingRecipeIndex < 0 || m_ShoppingRecipeIndex >= (int) rHandle.data().getShoppingRecipesCount())
	{
		return nullptr;
	}
	return &rHandle.data().getShoppingRecipeAt(m_ShoppingRecipeIndex);
}

const recipebook::ShoppingRecipe* ListModelShoppingListItems::getShoppingRecipe(recipebook::RBDataReadHandle& rHandle) const
{
	if(m_ShoppingRecipeIndex < 0 || m_ShoppingRecipeIndex >= (int) rHandle.data().getShoppingRecipesCount())
	{
		return nullptr;
	}
	return &rHandle.data().getShoppingRecipeAt(m_ShoppingRecipeIndex);
}

int ListModelShoppingListItems::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);

	if(m_ShoppingRecipeIndex < 0)
	{
		return 0;
	}

	RBDataReadHandle handle(m_rRBDataHandler);
	const ShoppingRecipe* pRecipe = getShoppingRecipe(handle);
	if(pRecipe == nullptr)
		return 0;

	return pRecipe->getItemsCount();
}

QVariant ListModelShoppingListItems::data(const QModelIndex& index, int iRole) const
{
	if(m_ShoppingRecipeIndex < 0)
	{
		return QVariant();
	}

	ShoppingListItemsRoles role = static_cast<ShoppingListItemsRoles>(iRole);
	if(role == ShoppingListItemsRoles::NameRole)
	{
		return name(index.row());
	}
	else if(role == ShoppingListItemsRoles::AmountUnitRole)
	{
		return amountUnit(index.row());
	}
	else if(role == ShoppingListItemsRoles::AmountIsRangeRole)
	{
		return amountIsRange(index.row());
	}
	else if(role == ShoppingListItemsRoles::AmountMinRole)
	{
		return amountMin(index.row());
	}
	else if(role == ShoppingListItemsRoles::AmountMaxRole)
	{
		return amountMax(index.row());
	}
	else if(role == ShoppingListItemsRoles::AdditionalInfoRole)
	{
		return additionalInfo(index.row());
	}
	else if(role == ShoppingListItemsRoles::SizeRole)
	{
		return sizeIndex(index.row());
	}
	else if(role == ShoppingListItemsRoles::OptionalRole)
	{
		return optional(index.row());
	}
	else if(role == ShoppingListItemsRoles::HasGroupRole)
	{
		return hasGroup(index.row());
	}
	else if(role == ShoppingListItemsRoles::GroupRole)
	{
		return group(index.row());
	}
	else if(role == ShoppingListItemsRoles::GroupColorRole)
	{
		return groupColor(index.row());
	}
	else if(role == ShoppingListItemsRoles::StatusRole)
	{
		return status(index.row());
	}
	else if(role == ShoppingListItemsRoles::ItemEnabledRole)
	{
		return isItemEnabled(index.row());
	}

	return QVariant();
}

bool ListModelShoppingListItems::setData(const QModelIndex& index, const QVariant& value, int iRole)
{
	if(m_ShoppingRecipeIndex < 0)
	{
		return false;
	}

	ShoppingListItemsRoles role = static_cast<ShoppingListItemsRoles>(iRole);
	if(role == ShoppingListItemsRoles::NameRole)
	{
		return false;
	}
	else if(role == ShoppingListItemsRoles::AmountUnitRole)
	{
		setAmountUnit(index.row(), value.toUInt());
		return true;
	}
	else if(role == ShoppingListItemsRoles::AmountIsRangeRole)
	{
		setAmountIsRange(index.row(), value.toBool());
		return true;
	}
	else if(role == ShoppingListItemsRoles::AmountMinRole)
	{
		setAmountMin(index.row(), value.toFloat());
		return true;
	}
	else if(role == ShoppingListItemsRoles::AmountMaxRole)
	{
		setAmountMax(index.row(), value.toFloat());
		return true;
	}
	else if(role == ShoppingListItemsRoles::AdditionalInfoRole)
	{
		setAdditionalInfo(index.row(), value.toString());
		return true;
	}
	else if(role == ShoppingListItemsRoles::SizeRole)
	{
		setSizeIndex(index.row(), value.toUInt());
		return true;
	}
	else if(role == ShoppingListItemsRoles::OptionalRole)
	{
		setOptional(index.row(), value.toBool());
		return true;
	}
	else if(role == ShoppingListItemsRoles::StatusRole)
	{
		setStatus(index.row(), value.toUInt());
		return true;
	}
	else if(role == ShoppingListItemsRoles::ItemEnabledRole)
	{
		setItemEnabled(index.row(), value.toBool());
		return true;
	}

	return false;
}

QHash<int, QByteArray> ListModelShoppingListItems::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[(int)ShoppingListItemsRoles::NameRole] = "name";
	roles[(int)ShoppingListItemsRoles::AmountUnitRole] = "amountUnit";
	roles[(int)ShoppingListItemsRoles::AmountIsRangeRole] = "amountIsRange";
	roles[(int)ShoppingListItemsRoles::AmountMinRole] = "amountMin";
	roles[(int)ShoppingListItemsRoles::AmountMaxRole] = "amountMax";
	roles[(int)ShoppingListItemsRoles::AdditionalInfoRole] = "additionalInfo";
	roles[(int)ShoppingListItemsRoles::SizeRole] = "sizeIndex";
	roles[(int)ShoppingListItemsRoles::OptionalRole] = "optional";
	roles[(int)ShoppingListItemsRoles::HasGroupRole] = "hasGroup";
	roles[(int)ShoppingListItemsRoles::GroupRole] = "group";
	roles[(int)ShoppingListItemsRoles::GroupColorRole] = "groupColor";
	roles[(int)ShoppingListItemsRoles::StatusRole] = "status";
	roles[(int)ShoppingListItemsRoles::ItemEnabledRole] = "itemEnabled";
	return roles;
}

void ListModelShoppingListItems::setDataChanged(int row, ShoppingListItemsRoles role)
{
	QVector<int> rolesChanged;
	rolesChanged.append((int)role);
	dataChanged(index(row), index(row), rolesChanged);
}

QString ListModelShoppingListItems::name(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int)pRecipe->getItemsCount())
		return "";

	const ShoppingListItem& rItem = pRecipe->getItemAt(row);
	return rItem.getName();
}

bool ListModelShoppingListItems::firstInGroup(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int)pRecipe->getItemsCount())
		return "";

	const ShoppingListItem& rItem = pRecipe->getItemAt(row);

	if(!rItem.hasAlternativesGroup())
	{
		return false;
	}

	if(row == 0)
	{
		return rItem.hasAlternativesGroup();
	}

	const ShoppingListItem& rPrevItem = pRecipe->getItemAt(row - 1);
	if(!rPrevItem.hasAlternativesGroup())
	{
		return true;
	}
	return &rPrevItem.getAlternativesGroup() != &rItem.getAlternativesGroup();
}

bool ListModelShoppingListItems::lastInGroup(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int)pRecipe->getItemsCount())
		return "";

	const ShoppingListItem& rItem = pRecipe->getItemAt(row);

	if(!rItem.hasAlternativesGroup())
	{
		return false;
	}

	if(row == (int)pRecipe->getItemsCount() - 1)
	{
		return rItem.hasAlternativesGroup();
	}
	const ShoppingListItem& rNextItem = pRecipe->getItemAt(row + 1);
	if(!rNextItem.hasAlternativesGroup())
	{
		return true;
	}
	return &rNextItem.getAlternativesGroup() != &rItem.getAlternativesGroup();
}

bool ListModelShoppingListItems::hasGroup(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int)pRecipe->getItemsCount())
		return "";

	const ShoppingListItem& rItem = pRecipe->getItemAt(row);

	return rItem.hasAlternativesGroup();
}

quint32 ListModelShoppingListItems::indexUnitUnitless() const
{
	return (quint32)Unit::Unitless;
}

quint32 ListModelShoppingListItems::amountUnit(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int)pRecipe->getItemsCount())
		return 0;

	const ShoppingListItem& rItem = pRecipe->getItemAt(row);
	return (quint32)rItem.getAmount().getUnit();
}

float ListModelShoppingListItems::amountMin(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int)pRecipe->getItemsCount())
		return -1.0f;

	const ShoppingListItem& rItem = pRecipe->getItemAt(row);
	return rItem.getAmount().getQuantityMin();
}

float ListModelShoppingListItems::amountMax(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int)pRecipe->getItemsCount())
		return -1.0f;

	const ShoppingListItem& rItem = pRecipe->getItemAt(row);
	return rItem.getAmount().getQuantityMax();
}

bool ListModelShoppingListItems::amountIsRange(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int) pRecipe->getItemsCount())
		return false;

	const ShoppingListItem& rItem = pRecipe->getItemAt(row);
	return rItem.getAmount().isRange();
}

QString ListModelShoppingListItems::additionalInfo(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int)pRecipe->getItemsCount())
		return "";

	const ShoppingListItem& rItem = pRecipe->getItemAt(row);
	return rItem.getAdditionalInfo();
}

quint32 ListModelShoppingListItems::sizeIndex(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int)pRecipe->getItemsCount())
		return 1;

	const ShoppingListItem& rItem = pRecipe->getItemAt(row);
	return (int)rItem.getSize();
}

bool ListModelShoppingListItems::optional(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int)pRecipe->getItemsCount())
		return false;

	const ShoppingListItem& rItem = pRecipe->getItemAt(row);
	return rItem.isOptional();
}

QString ListModelShoppingListItems::group(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int)pRecipe->getItemsCount())
		return "";

	const ShoppingListItem& rItem = pRecipe->getItemAt(row);

	if(rItem.hasAlternativesGroup())
	{
		return rItem.getAlternativesGroup().getName();
	}

	return m_rConverter.getStringNoAlternativesGroup();
}

QString ListModelShoppingListItems::groupColor(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int)pRecipe->getItemsCount())
		return "";

	const ShoppingListItem& rItem = pRecipe->getItemAt(row);

	if(rItem.hasAlternativesGroup())
	{
		return rItem.getAlternativesGroup().getColor().name();
	}

	return "";
}

quint32 ListModelShoppingListItems::status(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int)pRecipe->getItemsCount())
		return 0;

	const ShoppingListItem& rItem = pRecipe->getItemAt(row);
	return (quint32)rItem.getStatus();
}
    
bool ListModelShoppingListItems::isItemEnabled(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int)pRecipe->getItemsCount())
		return 0;

	const ShoppingListItem& rItem = pRecipe->getItemAt(row);
	return rItem.isItemEnabled();
}

void ListModelShoppingListItems::setAmountUnit(int row, quint32 uiUnit)
{
	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

		if(pRecipe == nullptr || row < 0 || row >= (int) pRecipe->getItemsCount())
			return;

		ShoppingListItem& rItem = pRecipe->getItemAt(row);
		rItem.getAmount().setUnit((Unit) uiUnit);
	}

	setDataChanged(row, ShoppingListItemsRoles::AmountUnitRole);
}

void ListModelShoppingListItems::setAmountMin(int row, float amount)
{
	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

		if(pRecipe == nullptr || row < 0 || row >= (int) pRecipe->getItemsCount())
			return;

		ShoppingListItem& rItem = pRecipe->getItemAt(row);
		rItem.getAmount().setQuantityMin(amount);
	}

	setDataChanged(row, ShoppingListItemsRoles::AmountMinRole);
}

void ListModelShoppingListItems::setAmountMax(int row, float amount)
{
	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

		if(pRecipe == nullptr || row < 0 || row >= (int) pRecipe->getItemsCount())
			return;

		ShoppingListItem& rItem = pRecipe->getItemAt(row);
		rItem.getAmount().setQuantityMax(amount);
	}

	setDataChanged(row, ShoppingListItemsRoles::AmountMaxRole);
}

void ListModelShoppingListItems::setAmountIsRange(int row, bool bRange)
{
	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

		if(pRecipe == nullptr || row < 0 || row >= (int) pRecipe->getItemsCount())
			return;

		ShoppingListItem& rItem = pRecipe->getItemAt(row);
		rItem.getAmount().setIsRange(bRange);
	}

	setDataChanged(row, ShoppingListItemsRoles::AmountIsRangeRole);
	setDataChanged(row, ShoppingListItemsRoles::AmountMaxRole);
}

void ListModelShoppingListItems::setAdditionalInfo(int row, QString text)
{
	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

		if(pRecipe == nullptr || row < 0 || row >= (int) pRecipe->getItemsCount())
			return;

		ShoppingListItem& rItem = pRecipe->getItemAt(row);
		rItem.setAdditionInfo(text);
	}

	setDataChanged(row, ShoppingListItemsRoles::AdditionalInfoRole);
}

void ListModelShoppingListItems::setSizeIndex(int row, quint32 index)
{
	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

		if(pRecipe == nullptr || row < 0 || row >= (int) pRecipe->getItemsCount())
			return;

		ShoppingListItem& rItem = pRecipe->getItemAt(row);
		rItem.setSize((Size) index);
	}

	setDataChanged(row, ShoppingListItemsRoles::SizeRole);
}

void ListModelShoppingListItems::setOptional(int row, bool bOptional)
{
	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

		if(pRecipe == nullptr || row < 0 || row >= (int) pRecipe->getItemsCount())
			return;

		ShoppingListItem& rItem = pRecipe->getItemAt(row);
		rItem.setIsOptional(bOptional);
	}

	setDataChanged(row, ShoppingListItemsRoles::OptionalRole);
}

void ListModelShoppingListItems::setGroup(int row, QString group)
{
	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

		if(pRecipe == nullptr || row < 0 || row >= (int) pRecipe->getItemsCount())
			return;

		ShoppingListItem& rItem = pRecipe->getItemAt(row);
		if(group == m_rConverter.getStringNoAlternativesGroup())
		{
			rItem.resetAlternativesGroup();
		}
		else if(handle.data().existsAlternativesType(group))
		{
			rItem.setAlternativesGroup(handle.data().getAlternativesType(group));

		}
		else
		{
			return;
		}
	}

	setDataChanged(row, ShoppingListItemsRoles::GroupRole);
	setDataChanged(row, ShoppingListItemsRoles::GroupColorRole);
}
        
void ListModelShoppingListItems::setStatus(int row, quint32 uiStatus)
{
	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

		if(pRecipe == nullptr || row < 0 || row >= (int) pRecipe->getItemsCount())
			return;

		ShoppingListItem& rItem = pRecipe->getItemAt(row);
		rItem.setStatus((Status) uiStatus);
	}

	setDataChanged(row, ShoppingListItemsRoles::StatusRole);
}

void ListModelShoppingListItems::setItemEnabled(int row, bool bEnabled)
{
	bool bItemWithGroup = false;
	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

		if(pRecipe == nullptr || row < 0 || row >= (int) pRecipe->getItemsCount())
			return;

		ShoppingListItem& rItem = pRecipe->getItemAt(row);
		rItem.setItemEnabled(bEnabled);
		bItemWithGroup = rItem.hasAlternativesGroup();
	}

	setDataChanged(row, ShoppingListItemsRoles::ItemEnabledRole);

	if(bItemWithGroup)
	{
		emit shoppingitemEnabledChanged(m_ShoppingRecipeIndex);
	}
}

bool ListModelShoppingListItems::canItemsBeAdded() const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const ShoppingRecipe* pRecipe = getShoppingRecipe(handle);
	if(pRecipe == nullptr)
	{
		return false;
	}
	return handle.data().getIngredientsCount() > 0;
}

int ListModelShoppingListItems::addItem(QString strIngredient)
{
	if(!canItemsBeAdded())
	{
		return -1;
	}

	qint32 index = -1;
	{
		RBDataReadHandle handle(m_rRBDataHandler);
		const ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

		if(pRecipe == nullptr || !handle.data().existsIngredient(strIngredient))
		{
			return -1;
		}

		const Ingredient& rIngredient = handle.data().getIngredient(strIngredient);
		if(pRecipe->existsItem(rIngredient))
		{
			return -1;
		}

		index = pRecipe->getItemIndex(rIngredient);
	}

	beginInsertRows(QModelIndex(), index, index);

	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		ShoppingRecipe* pRecipe = getShoppingRecipe(handle);
		const Ingredient& rIngredient = handle.data().getIngredient(strIngredient);
		pRecipe->addItem(rIngredient);
	}

	endInsertRows();

	return index;
}

bool ListModelShoppingListItems::removeItem(int row)
{
	bool bSuccess = false;
	{
		RBDataReadHandle handle(m_rRBDataHandler);
		const ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

		if(pRecipe == nullptr || row < 0 || row >= (int) pRecipe->getItemsCount())
			return false;
	}

	beginRemoveRows(QModelIndex(), row, row);

	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		ShoppingRecipe* pRecipe = getShoppingRecipe(handle);
		const ShoppingListItem& rItem = pRecipe->getItemAt(row);
		bSuccess = pRecipe->removeItem(rItem);
	}

	endRemoveRows();

	return bSuccess;
}

void ListModelShoppingListItems::beginEditList()
{
	m_EditListSelectedValues.clear();
	m_EditListDeselectedValues.clear();
}

bool ListModelShoppingListItems::itemSelected(QString itemName) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

	if(pRecipe == nullptr || !handle.data().existsIngredient(itemName))
	{
		return false;
	}

	if(m_EditListSelectedValues.contains(itemName))
	{
		return true;
	}

	if(m_EditListDeselectedValues.contains(itemName))
	{
		return false;
	}

	const Ingredient& rIngredient = handle.data().getIngredient(itemName);
	return pRecipe->existsItem(rIngredient);
}

void ListModelShoppingListItems::changeState(QString itemName, bool selected)
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

	if(pRecipe == nullptr || !handle.data().existsIngredient(itemName))
	{
		return;
	}

	const Ingredient& rIngredient = handle.data().getIngredient(itemName);

	if(selected)
	{
		m_EditListDeselectedValues.removeOne(itemName);
		if(!pRecipe->existsItem(rIngredient))
		{
			m_EditListSelectedValues.append(itemName);
		}
	}
	else
	{
		m_EditListSelectedValues.removeOne(itemName);
		if(pRecipe->existsItem(rIngredient))
		{
			m_EditListDeselectedValues.append(itemName);
		}
	}
}

void ListModelShoppingListItems::cancelEditList()
{
	m_EditListSelectedValues.clear();
	m_EditListDeselectedValues.clear();
}

bool ListModelShoppingListItems::applyEditList()
{
	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		ShoppingRecipe* pRecipe = getShoppingRecipe(handle);

		if(pRecipe == nullptr)
		{
			return false;
		}

		for(QString item : qAsConst(m_EditListDeselectedValues))
		{
			const Ingredient& rIngredient = handle.data().getIngredient(item);
			const ShoppingListItem& rRecipeItem = pRecipe->getItem(rIngredient);
			pRecipe->removeItem(rRecipeItem);
		}
	}

	for(QString item : qAsConst(m_EditListSelectedValues))
	{
		addItem(item);
	}

	bool bListChanged = m_EditListSelectedValues.size() > 0 || m_EditListDeselectedValues.size() > 0;

	m_EditListSelectedValues.clear();
	m_EditListDeselectedValues.clear();

	return bListChanged;
}

void ListModelShoppingListItems::onDependentItemChanged(quint32 row)
{
	Q_UNUSED(row);

	if(m_ShoppingRecipeIndex >= 0)
	{
		dataChanged(index(0), index(rowCount()-1));
	}
}
