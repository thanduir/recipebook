#include "ListModelRecipeItems.h"
#include <QException>
#include <data/RecipeBook.h>
#include <data/Recipe.h>
#include <data/RecipeItem.h>
#include <data/AlternativesType.h>
#include "uistringconverter.h"
#include "RBDataHandler.h"

using namespace recipebook::UI;

ListModelRecipeItems::ListModelRecipeItems(recipebook::RBDataHandler& rRBDataHandler, const UIStringConverter& rConverter)
:	m_rRBDataHandler(rRBDataHandler),
	m_rConverter(rConverter),
	m_RecipeIndex(-1)
{
}

void ListModelRecipeItems::setRecipe(int row)
{
	if(m_RecipeIndex == row)
	{
		return;
	}

	beginResetModel();
	m_RecipeIndex = row;
	endResetModel();
}

recipebook::Recipe* ListModelRecipeItems::getRecipe(recipebook::RBDataWriteHandle& rHandle)
{
	if(m_RecipeIndex < 0 || m_RecipeIndex >= (int) rHandle.data().getRecipesCount())
	{
		return nullptr;
	}
	return &rHandle.data().getRecipeAt(m_RecipeIndex);
}

const recipebook::Recipe* ListModelRecipeItems::getRecipe(recipebook::RBDataReadHandle& rHandle) const
{
	if(m_RecipeIndex < 0 || m_RecipeIndex >= (int) rHandle.data().getRecipesCount())
	{
		return nullptr;
	}
	return &rHandle.data().getRecipeAt(m_RecipeIndex);
}

int ListModelRecipeItems::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);

	if(m_RecipeIndex < 0)
	{
		return 0;
	}

	RBDataReadHandle handle(m_rRBDataHandler);
	const Recipe* pRecipe = getRecipe(handle);

	return pRecipe->getRecipeItemsCount();
}

QVariant ListModelRecipeItems::data(const QModelIndex& index, int iRole) const
{
	if(m_RecipeIndex < 0)
	{
		return QVariant();
	}

	RecipeItemsRoles role = static_cast<RecipeItemsRoles>(iRole);
	if(role == RecipeItemsRoles::NameRole)
	{
		return name(index.row());
	}
	else if(role == RecipeItemsRoles::AmountUnitRole)
	{
		return amountUnit(index.row());
	}
	else if(role == RecipeItemsRoles::AmountIsRangeRole)
	{
		return amountIsRange(index.row());
	}
	else if(role == RecipeItemsRoles::AmountMinRole)
	{
		return amountMin(index.row());
	}
	else if(role == RecipeItemsRoles::AmountMaxRole)
	{
		return amountMax(index.row());
	}
	else if(role == RecipeItemsRoles::AdditionalInfoRole)
	{
		return additionalInfo(index.row());
	}
	else if(role == RecipeItemsRoles::SizeRole)
	{
		return sizeIndex(index.row());
	}
	else if(role == RecipeItemsRoles::OptionalRole)
	{
		return optional(index.row());
	}
	else if(role == RecipeItemsRoles::GroupRole)
	{
		return group(index.row());
	}
	else if(role == RecipeItemsRoles::GroupColorRole)
	{
		return groupColor(index.row());
	}

	return QVariant();
}

bool ListModelRecipeItems::setData(const QModelIndex& index, const QVariant& value, int iRole)
{
	if(m_RecipeIndex < 0)
	{
		return false;
	}

	RecipeItemsRoles role = static_cast<RecipeItemsRoles>(iRole);
	if(role == RecipeItemsRoles::NameRole)
	{
		return false;
	}
	else if(role == RecipeItemsRoles::AmountUnitRole)
	{
		setAmountUnit(index.row(), value.toUInt());
		return true;
	}
	else if(role == RecipeItemsRoles::AmountIsRangeRole)
	{
		setAmountIsRange(index.row(), value.toBool());
		return true;
	}
	else if(role == RecipeItemsRoles::AmountMinRole)
	{
		setAmountMin(index.row(), value.toFloat());
		return true;
	}
	else if(role == RecipeItemsRoles::AmountMaxRole)
	{
		setAmountMax(index.row(), value.toFloat());
		return true;
	}
	else if(role == RecipeItemsRoles::AdditionalInfoRole)
	{
		setAdditionalInfo(index.row(), value.toString());
		return true;
	}
	else if(role == RecipeItemsRoles::SizeRole)
	{
		setSizeIndex(index.row(), value.toUInt());
		return true;
	}
	else if(role == RecipeItemsRoles::OptionalRole)
	{
		setOptional(index.row(), value.toBool());
		return true;
	}
	else if(role == RecipeItemsRoles::GroupRole)
	{
		setGroup(index.row(), value.toString());
		return true;
	}

	return false;
}

QHash<int, QByteArray> ListModelRecipeItems::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[(int)RecipeItemsRoles::NameRole] = "name";
	roles[(int)RecipeItemsRoles::AmountUnitRole] = "amountUnit";
	roles[(int)RecipeItemsRoles::AmountIsRangeRole] = "amountIsRange";
	roles[(int)RecipeItemsRoles::AmountMinRole] = "amountMin";
	roles[(int)RecipeItemsRoles::AmountMaxRole] = "amountMax";
	roles[(int)RecipeItemsRoles::AdditionalInfoRole] = "additionalInfo";
	roles[(int)RecipeItemsRoles::SizeRole] = "sizeIndex";
	roles[(int)RecipeItemsRoles::OptionalRole] = "optional";
	roles[(int)RecipeItemsRoles::GroupRole] = "group";
	roles[(int)RecipeItemsRoles::GroupColorRole] = "groupColor";
	return roles;
}

void ListModelRecipeItems::setDataChanged(int row, RecipeItemsRoles role)
{
	QVector<int> rolesChanged;
	rolesChanged.append((int)role);
	dataChanged(index(row), index(row), rolesChanged);
}

QString ListModelRecipeItems::name(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const Recipe* pRecipe = getRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int)pRecipe->getRecipeItemsCount())
		return "";

	const RecipeItem& rItem = pRecipe->getRecipeItemAt(row);
	return rItem.getName();
}

quint32 ListModelRecipeItems::indexUnitUnitless() const
{
	return (quint32)Unit::Unitless;
}

quint32 ListModelRecipeItems::amountUnit(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const Recipe* pRecipe = getRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int)pRecipe->getRecipeItemsCount())
		return 0;

	const RecipeItem& rItem = pRecipe->getRecipeItemAt(row);
	return (quint32)rItem.getAmount().getUnit();
}

float ListModelRecipeItems::amountMin(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const Recipe* pRecipe = getRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int)pRecipe->getRecipeItemsCount())
		return -1.0f;

	const RecipeItem& rItem = pRecipe->getRecipeItemAt(row);
	return rItem.getAmount().getQuantityMin();
}

float ListModelRecipeItems::amountMax(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const Recipe* pRecipe = getRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int)pRecipe->getRecipeItemsCount())
		return -1.0f;

	const RecipeItem& rItem = pRecipe->getRecipeItemAt(row);
	return rItem.getAmount().getQuantityMax();
}

bool ListModelRecipeItems::amountIsRange(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const Recipe* pRecipe = getRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int) pRecipe->getRecipeItemsCount())
		return false;

	const RecipeItem& rItem = pRecipe->getRecipeItemAt(row);
	return rItem.getAmount().isRange();
}

QString ListModelRecipeItems::additionalInfo(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const Recipe* pRecipe = getRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int)pRecipe->getRecipeItemsCount())
		return "";

	const RecipeItem& rItem = pRecipe->getRecipeItemAt(row);
	return rItem.getAdditionalInfo();
}

quint32 ListModelRecipeItems::sizeIndex(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const Recipe* pRecipe = getRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int)pRecipe->getRecipeItemsCount())
		return 1;

	const RecipeItem& rItem = pRecipe->getRecipeItemAt(row);
	return (int)rItem.getSize();
}

bool ListModelRecipeItems::optional(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const Recipe* pRecipe = getRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int)pRecipe->getRecipeItemsCount())
		return false;

	const RecipeItem& rItem = pRecipe->getRecipeItemAt(row);
	return rItem.isOptional();
}

QString ListModelRecipeItems::group(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const Recipe* pRecipe = getRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int)pRecipe->getRecipeItemsCount())
		return "";

	const RecipeItem& rItem = pRecipe->getRecipeItemAt(row);

	if(rItem.hasAlternativesGroup())
	{
		return rItem.getAlternativesGroup().getName();
	}

	return m_rConverter.getStringNoAlternativesGroup();
}

QString ListModelRecipeItems::groupColor(int row) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const Recipe* pRecipe = getRecipe(handle);

	if(pRecipe == nullptr || row < 0 || row >= (int)pRecipe->getRecipeItemsCount())
		return "";

	const RecipeItem& rItem = pRecipe->getRecipeItemAt(row);

	if(rItem.hasAlternativesGroup())
	{
		return rItem.getAlternativesGroup().getColor().name();
	}

	return "";
}
        
void ListModelRecipeItems::setAmountUnit(int row, quint32 uiUnit)
{
	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		Recipe* pRecipe = getRecipe(handle);

		if(pRecipe == nullptr || row < 0 || row >= (int) pRecipe->getRecipeItemsCount())
			return;

		RecipeItem& rItem = pRecipe->getRecipeItemAt(row);
		rItem.getAmount().setUnit((Unit) uiUnit);
	}

	setDataChanged(row, RecipeItemsRoles::AmountUnitRole);
}

void ListModelRecipeItems::setAmountMin(int row, float amount)
{
	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		Recipe* pRecipe = getRecipe(handle);

		if(pRecipe == nullptr || row < 0 || row >= (int) pRecipe->getRecipeItemsCount())
			return;

		RecipeItem& rItem = pRecipe->getRecipeItemAt(row);
		rItem.getAmount().setQuantityMin(amount);
	}

	setDataChanged(row, RecipeItemsRoles::AmountMinRole);
}

void ListModelRecipeItems::setAmountMax(int row, float amount)
{
	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		Recipe* pRecipe = getRecipe(handle);

		if(pRecipe == nullptr || row < 0 || row >= (int) pRecipe->getRecipeItemsCount())
			return;

		RecipeItem& rItem = pRecipe->getRecipeItemAt(row);
		rItem.getAmount().setQuantityMax(amount);
	}

	setDataChanged(row, RecipeItemsRoles::AmountMaxRole);
}

void ListModelRecipeItems::setAmountIsRange(int row, bool bRange)
{
	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		Recipe* pRecipe = getRecipe(handle);

		if(pRecipe == nullptr || row < 0 || row >= (int) pRecipe->getRecipeItemsCount())
			return;

		RecipeItem& rItem = pRecipe->getRecipeItemAt(row);
		rItem.getAmount().setIsRange(bRange);
	}

	setDataChanged(row, RecipeItemsRoles::AmountIsRangeRole);
	setDataChanged(row, RecipeItemsRoles::AmountMaxRole);
}

void ListModelRecipeItems::setAdditionalInfo(int row, QString text)
{
	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		Recipe* pRecipe = getRecipe(handle);

		if(pRecipe == nullptr || row < 0 || row >= (int) pRecipe->getRecipeItemsCount())
			return;

		RecipeItem& rItem = pRecipe->getRecipeItemAt(row);
		rItem.setAdditionInfo(text);
	}

	setDataChanged(row, RecipeItemsRoles::AdditionalInfoRole);
}

void ListModelRecipeItems::setSizeIndex(int row, quint32 index)
{
	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		Recipe* pRecipe = getRecipe(handle);

		if(pRecipe == nullptr || row < 0 || row >= (int) pRecipe->getRecipeItemsCount())
			return;

		RecipeItem& rItem = pRecipe->getRecipeItemAt(row);
		rItem.setSize((Size) index);
	}

	setDataChanged(row, RecipeItemsRoles::SizeRole);
}

void ListModelRecipeItems::setOptional(int row, bool bOptional)
{
	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		Recipe* pRecipe = getRecipe(handle);

		if(pRecipe == nullptr || row < 0 || row >= (int) pRecipe->getRecipeItemsCount())
			return;

		RecipeItem& rItem = pRecipe->getRecipeItemAt(row);
		rItem.setIsOptional(bOptional);
	}

	setDataChanged(row, RecipeItemsRoles::OptionalRole);
}

int ListModelRecipeItems::setGroup(int row, QString group)
{
	int newRow = row;
	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		Recipe* pRecipe = getRecipe(handle);

		if(pRecipe == nullptr || row < 0 || row >= (int) pRecipe->getRecipeItemsCount())
			return -1;

		RecipeItem& rItem = pRecipe->getRecipeItemAt(row);
		if(group == m_rConverter.getStringNoAlternativesGroup())
		{
			rItem.resetAlternativesGroup();
			if(!pRecipe->moveGroupItemsTogether())
			{
				newRow = pRecipe->getRecipeItemIndex(rItem.getName());
			}
		}
		else if(handle.data().existsAlternativesType(group))
		{
			rItem.setAlternativesGroup(handle.data().getAlternativesType(group));
			if(!pRecipe->moveGroupItemsTogether())
			{
				newRow = pRecipe->getRecipeItemIndex(rItem.getName());
			}
		}
		else
		{
			return row;
		}
	}

	if(newRow != row)
	{
		dataChanged(index(0), index(rowCount(QModelIndex())-1));
	}
	else
	{
		setDataChanged(row, RecipeItemsRoles::GroupRole);
		setDataChanged(row, RecipeItemsRoles::GroupColorRole);
	}

	return newRow;
}

int ListModelRecipeItems::addRecipeItem(QString strIngredient)
{
	qint32 index = -1;
	{
		RBDataReadHandle handle(m_rRBDataHandler);
		const Recipe* pRecipe = getRecipe(handle);

		if(pRecipe == nullptr || !handle.data().existsIngredient(strIngredient))
		{
			return -1;
		}

		index = pRecipe->getRecipeItemsCount();
	}

	beginInsertRows(QModelIndex(), index, index);

	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		Recipe* pRecipe = getRecipe(handle);
		const Ingredient& rIngredient = handle.data().getIngredient(strIngredient);
		pRecipe->addRecipeItem(rIngredient);
	}

	endInsertRows();

	return index;
}

bool ListModelRecipeItems::removeItem(int row)
{
	bool bSuccess = false;
	{
		RBDataReadHandle handle(m_rRBDataHandler);
		const Recipe* pRecipe = getRecipe(handle);

		if(pRecipe == nullptr || row < 0 || row >= (int) pRecipe->getRecipeItemsCount())
			return false;
	}

	beginRemoveRows(QModelIndex(), row, row);

	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		Recipe* pRecipe = getRecipe(handle);
		const RecipeItem& rItem = pRecipe->getRecipeItemAt(row);
		bSuccess = pRecipe->removeRecipeItem(rItem);
	}

	endRemoveRows();

	return bSuccess;
}

void ListModelRecipeItems::moveItem(int row, int target)
{
	{
		RBDataReadHandle handle(m_rRBDataHandler);
		const Recipe* pRecipe = getRecipe(handle);

		if(pRecipe == nullptr
		   || row < 0 || row >= (int) pRecipe->getRecipeItemsCount()
		   || target < 0 || target >= (int) pRecipe->getRecipeItemsCount()
		   || row == target)
			return;
	}

	beginMoveRows(QModelIndex(), row, row, QModelIndex(), target > row ? target + 1 : target);

	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		Recipe* pRecipe = getRecipe(handle);
		pRecipe->moveRecipeItem(pRecipe->getRecipeItemAt(row), target);
	}

	// endMoveRows calls rowCount which needs a read handle -> I can do this only after moving
	endMoveRows();
}

void ListModelRecipeItems::onDependentItemChanged(quint32 row)
{
	if(m_RecipeIndex >= 0)
	{
		dataChanged(index(0), index(rowCount()-1));
	}
}

void ListModelRecipeItems::beginEditList()
{
	m_EditListSelectedValues.clear();
	m_EditListDeselectedValues.clear();
}

bool ListModelRecipeItems::itemSelected(QString itemName) const
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const Recipe* pRecipe = getRecipe(handle);

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
	return pRecipe->existsRecipeItem(rIngredient);
}

void ListModelRecipeItems::changeState(QString itemName, bool selected)
{
	RBDataReadHandle handle(m_rRBDataHandler);
	const Recipe* pRecipe = getRecipe(handle);

	if(pRecipe == nullptr || !handle.data().existsIngredient(itemName))
	{
		return;
	}

	const Ingredient& rIngredient = handle.data().getIngredient(itemName);

	if(selected)
	{
		m_EditListDeselectedValues.removeOne(itemName);
		if(!pRecipe->existsRecipeItem(rIngredient))
		{
			m_EditListSelectedValues.append(itemName);
		}
	}
	else
	{
		m_EditListSelectedValues.removeOne(itemName);
		if(pRecipe->existsRecipeItem(rIngredient))
		{
			m_EditListDeselectedValues.append(itemName);
		}
	}
}

void ListModelRecipeItems::cancelEditList()
{
	m_EditListSelectedValues.clear();
	m_EditListDeselectedValues.clear();
}

bool ListModelRecipeItems::applyEditList()
{
	{
		RBDataWriteHandle handle(m_rRBDataHandler);
		Recipe* pRecipe = getRecipe(handle);

		if(pRecipe == nullptr)
		{
			return false;
		}

		for(QString item : qAsConst(m_EditListDeselectedValues))
		{
			const Ingredient& rIngredient = handle.data().getIngredient(item);
			const RecipeItem& rRecipeItem = pRecipe->getRecipeItem(rIngredient);
			pRecipe->removeRecipeItem(rRecipeItem);
		}
	}

	for(QString item : qAsConst(m_EditListSelectedValues))
	{
		addRecipeItem(item);
	}

	bool bListChanged = m_EditListSelectedValues.size() > 0 || m_EditListDeselectedValues.size() > 0;

	m_EditListSelectedValues.clear();
	m_EditListDeselectedValues.clear();

	return bListChanged;
}
