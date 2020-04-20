#include "ListModelRecipeItems.h"
#include <QException>
#include <data/RecipeBook.h>
#include <data/Recipe.h>
#include <data/RecipeItem.h>
#include <data/AlternativesType.h>
#include "uistringconverter.h"

using namespace recipebook::UI;

ListModelRecipeItems::ListModelRecipeItems(recipebook::RecipeBook& rRecipeBook, const UIStringConverter& rConverter)
:	m_rRecipeBook(rRecipeBook),
    m_rConverter(rConverter),
    m_pRecipe(nullptr)
{
}

void ListModelRecipeItems::setRecipe(int row)
{
    if (row < 0 || row >= (int)m_rRecipeBook.getRecipesCount())
        return;

    beginResetModel();
    m_pRecipe = &m_rRecipeBook.getRecipeAt(row);
    endResetModel();
}

int ListModelRecipeItems::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    if(m_pRecipe == nullptr)
    {
        return 0;
    }

    return m_pRecipe->getRecipeItemsCount();
}

QVariant ListModelRecipeItems::data(const QModelIndex& index, int iRole) const
{
    if(m_pRecipe == nullptr)
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
    if(m_pRecipe == nullptr)
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
    if(!m_pRecipe || row < 0 || row >= (int)m_pRecipe->getRecipeItemsCount())
        return "";

    const RecipeItem& rItem = m_pRecipe->getRecipeItemAt(row);
    return rItem.getName();
}

quint32 ListModelRecipeItems::indexUnitUnitless() const
{
    return (quint32)Unit::Unitless;
}

quint32 ListModelRecipeItems::amountUnit(int row) const
{
    if(!m_pRecipe || row < 0 || row >= (int)m_pRecipe->getRecipeItemsCount())
        return 0;

    const RecipeItem& rItem = m_pRecipe->getRecipeItemAt(row);
    return (quint32)rItem.getAmount().getUnit();
}

float ListModelRecipeItems::amountMin(int row) const
{
    if(!m_pRecipe || row < 0 || row >= (int)m_pRecipe->getRecipeItemsCount())
        return -1.0f;

    const RecipeItem& rItem = m_pRecipe->getRecipeItemAt(row);
    return rItem.getAmount().getQuantityMin();
}

float ListModelRecipeItems::amountMax(int row) const
{
    if(!m_pRecipe || row < 0 || row >= (int)m_pRecipe->getRecipeItemsCount())
        return -1.0f;

    const RecipeItem& rItem = m_pRecipe->getRecipeItemAt(row);
    return rItem.getAmount().getQuantityMax();
}

bool ListModelRecipeItems::amountIsRange(int row) const
{
    if(!m_pRecipe || row < 0 || row >= (int) m_pRecipe->getRecipeItemsCount())
        return false;

    const RecipeItem& rItem = m_pRecipe->getRecipeItemAt(row);
    return rItem.getAmount().isRange();
}

QString ListModelRecipeItems::additionalInfo(int row) const
{
    if(!m_pRecipe || row < 0 || row >= (int)m_pRecipe->getRecipeItemsCount())
        return "";

    const RecipeItem& rItem = m_pRecipe->getRecipeItemAt(row);
    return rItem.getAdditionalInfo();
}

quint32 ListModelRecipeItems::sizeIndex(int row) const
{
    if(!m_pRecipe || row < 0 || row >= (int)m_pRecipe->getRecipeItemsCount())
        return 1;

    const RecipeItem& rItem = m_pRecipe->getRecipeItemAt(row);
    return (int)rItem.getSize();
}

bool ListModelRecipeItems::optional(int row) const
{
    if(!m_pRecipe || row < 0 || row >= (int)m_pRecipe->getRecipeItemsCount())
        return false;

    const RecipeItem& rItem = m_pRecipe->getRecipeItemAt(row);
    return rItem.isOptional();
}

QString ListModelRecipeItems::group(int row) const
{
    if(!m_pRecipe || row < 0 || row >= (int)m_pRecipe->getRecipeItemsCount())
        return "";

    const RecipeItem& rItem = m_pRecipe->getRecipeItemAt(row);

    if(rItem.hasAlternativesGroup())
    {
        return rItem.getAlternativesGroup().getName();
    }

    return m_rConverter.getStringNoAlternativesGroup();
}

QString ListModelRecipeItems::groupColor(int row) const
{
    if(!m_pRecipe || row < 0 || row >= (int)m_pRecipe->getRecipeItemsCount())
        return "";

    const RecipeItem& rItem = m_pRecipe->getRecipeItemAt(row);

    if(rItem.hasAlternativesGroup())
    {
        return rItem.getAlternativesGroup().getColor().name();
    }

    return "";
}
        
void ListModelRecipeItems::setAmountUnit(int row, quint32 uiUnit)
{
    if(!m_pRecipe || row < 0 || row >= (int)m_pRecipe->getRecipeItemsCount())
        return;

    RecipeItem& rItem = m_pRecipe->getRecipeItemAt(row);
    rItem.getAmount().setUnit((Unit)uiUnit);
    setDataChanged(row, RecipeItemsRoles::AmountUnitRole);
}

void ListModelRecipeItems::setAmountMin(int row, float amount)
{
    if(!m_pRecipe || row < 0 || row >= (int)m_pRecipe->getRecipeItemsCount())
        return;

    RecipeItem& rItem = m_pRecipe->getRecipeItemAt(row);
    rItem.getAmount().setQuantityMin(amount);
    setDataChanged(row, RecipeItemsRoles::AmountMinRole);
}

void ListModelRecipeItems::setAmountMax(int row, float amount)
{
    if(!m_pRecipe || row < 0 || row >= (int)m_pRecipe->getRecipeItemsCount())
        return;

    RecipeItem& rItem = m_pRecipe->getRecipeItemAt(row);
    rItem.getAmount().setQuantityMax(amount);
    setDataChanged(row, RecipeItemsRoles::AmountMaxRole);
}

void ListModelRecipeItems::setAmountIsRange(int row, bool bRange)
{
    if(!m_pRecipe || row < 0 || row >= (int)m_pRecipe->getRecipeItemsCount())
        return;

    RecipeItem& rItem = m_pRecipe->getRecipeItemAt(row);
    rItem.getAmount().setIsRange(bRange);

    setDataChanged(row, RecipeItemsRoles::AmountIsRangeRole);
    setDataChanged(row, RecipeItemsRoles::AmountMaxRole);
}

void ListModelRecipeItems::setAdditionalInfo(int row, QString text)
{
    if(!m_pRecipe || row < 0 || row >= (int)m_pRecipe->getRecipeItemsCount())
        return;

    RecipeItem& rItem = m_pRecipe->getRecipeItemAt(row);
    rItem.setAdditionInfo(text);
    setDataChanged(row, RecipeItemsRoles::AdditionalInfoRole);
}

void ListModelRecipeItems::setSizeIndex(int row, quint32 index)
{
    if(!m_pRecipe || row < 0 || row >= (int)m_pRecipe->getRecipeItemsCount())
        return;

    RecipeItem& rItem = m_pRecipe->getRecipeItemAt(row);
    rItem.setSize((Size)index);
    setDataChanged(row, RecipeItemsRoles::SizeRole);
}

void ListModelRecipeItems::setOptional(int row, bool bOptional)
{
    if(!m_pRecipe || row < 0 || row >= (int)m_pRecipe->getRecipeItemsCount())
        return;

    RecipeItem& rItem = m_pRecipe->getRecipeItemAt(row);
    rItem.setIsOptional(bOptional);
    setDataChanged(row, RecipeItemsRoles::OptionalRole);
}

void ListModelRecipeItems::setGroup(int row, QString group)
{
    if(!m_pRecipe || row < 0 || row >= (int)m_pRecipe->getRecipeItemsCount())
        return;

    RecipeItem& rItem = m_pRecipe->getRecipeItemAt(row);
    if(group == m_rConverter.getStringNoAlternativesGroup())
    {
        rItem.resetAlternativesGroup();
        setDataChanged(row, RecipeItemsRoles::GroupRole);
    }
    else if(m_rRecipeBook.existsAlternativesType(group))
    {
        rItem.setAlternativesGroup(m_rRecipeBook.getAlternativesType(group));
        setDataChanged(row, RecipeItemsRoles::GroupRole);
    }
}

int ListModelRecipeItems::addRecipeItem(QString strIngredient)
{
    if(m_pRecipe == nullptr || !m_rRecipeBook.existsIngredient(strIngredient))
    {
        return -1;
    }

    const Ingredient& rIngredient = m_rRecipeBook.getIngredient(strIngredient);
    qint32 index = m_pRecipe->getRecipeItemsCount();

    beginInsertRows(QModelIndex(), index, index);

    RecipeItem& rItem = m_pRecipe->addRecipeItem(rIngredient);

    endInsertRows();

    return index;
}

bool ListModelRecipeItems::removeItem(int row)
{
    if(!m_pRecipe || row < 0 || row >= (int)m_pRecipe->getRecipeItemsCount())
        return false;

    const RecipeItem& rItem = m_pRecipe->getRecipeItemAt(row);

    beginRemoveRows(QModelIndex(), row, row);
    bool bSuccess = m_pRecipe->removeRecipeItem(rItem);
    endRemoveRows();

    return bSuccess;
}

void ListModelRecipeItems::onDependentItemChanged(quint32 row)
{
    if(m_pRecipe != nullptr)
    {
        dataChanged(index(0), index(rowCount()-1));
    }
}

void ListModelRecipeItems::beginEditList()
{
    m_EditListSelectedValues.clear();
    m_EditListDeselectedValues.clear();
}

bool ListModelRecipeItems::itemSelected(QString itemName)
{
    if(m_pRecipe == nullptr || !m_rRecipeBook.existsIngredient(itemName))
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

    const Ingredient& rIngredient = m_rRecipeBook.getIngredient(itemName);
    return m_pRecipe->existsRecipeItem(rIngredient);
}

void ListModelRecipeItems::changeState(QString itemName, bool selected)
{
    if(m_pRecipe == nullptr || !m_rRecipeBook.existsIngredient(itemName))
    {
        return;
    }

    const Ingredient& rIngredient = m_rRecipeBook.getIngredient(itemName);

    if(selected)
    {
        m_EditListDeselectedValues.removeOne(itemName);
        if(!m_pRecipe->existsRecipeItem(rIngredient))
        {
            m_EditListSelectedValues.append(itemName);
        }
    }
    else
    {
        m_EditListSelectedValues.removeOne(itemName);
        if(m_pRecipe->existsRecipeItem(rIngredient))
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
    if(m_pRecipe != nullptr)
    {
        for(QString item : qAsConst(m_EditListDeselectedValues))
        {
            const Ingredient& rIngredient = m_rRecipeBook.getIngredient(item);
            const RecipeItem& rRecipeItem = m_pRecipe->getRecipeItem(rIngredient);
            m_pRecipe->removeRecipeItem(rRecipeItem);
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

    return false;
}
