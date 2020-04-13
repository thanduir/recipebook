#include "ListModelRecipeItems.h"
#include <QException>
#include <data/RecipeBook.h>
#include <data/Recipe.h>
#include <data/RecipeItem.h>
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

    int currentCount = m_pRecipe->getRecipeItems().getItemsCount();
    for(quint32 i = 0; i < m_pRecipe->getAlternativesGroupsCount(); ++i)
    {
        currentCount += m_pRecipe->getAlternativesGroupAt(i).getItemsCount() + 1;
    }
    return currentCount;
}

void ListModelRecipeItems::getItemInfo(int row, ItemInfo& rInfo) const
{
    if(m_pRecipe == nullptr || row < 0)
    {
        rInfo.type = ItemType::None;
        return;
    }
    else if(row < (int)m_pRecipe->getRecipeItems().getItemsCount())
    {
        rInfo.type = ItemType::RegularItem;
        rInfo.itemIndex = row;
        return;
    }
    else
    {
        quint32 currentCount = m_pRecipe->getRecipeItems().getItemsCount();
        for(quint32 i = 0; i < m_pRecipe->getAlternativesGroupsCount(); ++i)
        {
            RecipeItemGroup& rGroup = m_pRecipe->getAlternativesGroupAt(i);

            if(row == currentCount)
            {
                rInfo.type = ItemType::GroupHeader;
                rInfo.itemIndex = 0;
                rInfo.pGroup = &rGroup;
                return;
            }
            currentCount += 1;
            
            if((quint32)row < currentCount + rGroup.getItemsCount())
            {
                rInfo.type = ItemType::GroupItem;
                rInfo.itemIndex = row - currentCount;
                rInfo.pGroup = &rGroup;
                return;
            }
            currentCount += rGroup.getItemsCount();
        }
    }

    rInfo.type = ItemType::None;
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
    ItemInfo info;
    getItemInfo(row, info);

    if(info.type == ItemType::None)
        return "";
    
    if(info.type == ItemType::RegularItem)
    {
        return m_pRecipe->getRecipeItems().getItemAt(info.itemIndex).getName();
    }
    else if(info.type == ItemType::GroupHeader)
    {
        return info.pGroup->getName();
    }
    else if(info.type == ItemType::GroupItem)
    {
        return info.pGroup->getItemAt(info.itemIndex).getName();
    }

    throw QException();
}

bool ListModelRecipeItems::isRegularItem(int row) const
{
    ItemInfo info;
    getItemInfo(row, info);
    return info.type == ItemType::RegularItem;
}

bool ListModelRecipeItems::isGroupHeader(int row) const
{
    ItemInfo info;
    getItemInfo(row, info);
    return info.type == ItemType::GroupHeader;
}

bool ListModelRecipeItems::isGroupItem(int row) const
{
    ItemInfo info;
    getItemInfo(row, info);
    return info.type == ItemType::GroupItem;
}

quint32 ListModelRecipeItems::indexUnitUnitless() const
{
    return (quint32)Unit::Unitless;
}

quint32 ListModelRecipeItems::amountUnit(int row) const
{
    ItemInfo info;
    getItemInfo(row, info);

    if(info.type == ItemType::RegularItem)
    {
        return (quint32)m_pRecipe->getRecipeItems().getItemAt(info.itemIndex).getAmount().getUnit();
    }
    else if(info.type == ItemType::GroupItem)
    {
        return  (quint32)info.pGroup->getItemAt(info.itemIndex).getAmount().getUnit();
    }

    return 0;
}

float ListModelRecipeItems::amountMin(int row) const
{
    ItemInfo info;
    getItemInfo(row, info);

    if(info.type == ItemType::RegularItem)
    {
        return m_pRecipe->getRecipeItems().getItemAt(info.itemIndex).getAmount().getQuantityMin();
    }
    else if(info.type == ItemType::GroupItem)
    {
        return info.pGroup->getItemAt(info.itemIndex).getAmount().getQuantityMin();
    }

    return -1.0f;
}

float ListModelRecipeItems::amountMax(int row) const
{
    ItemInfo info;
    getItemInfo(row, info);

    if(info.type == ItemType::RegularItem)
    {
        return m_pRecipe->getRecipeItems().getItemAt(info.itemIndex).getAmount().getQuantityMax();
    }
    else if(info.type == ItemType::GroupItem)
    {
        return info.pGroup->getItemAt(info.itemIndex).getAmount().getQuantityMax();
    }

    return -1.0f;
}

bool ListModelRecipeItems::amountIsRange(int row) const
{
    ItemInfo info;
    getItemInfo(row, info);

    if(info.type == ItemType::RegularItem)
    {
        return m_pRecipe->getRecipeItems().getItemAt(info.itemIndex).getAmount().isRange();
    }
    else if(info.type == ItemType::GroupItem)
    {
        return info.pGroup->getItemAt(info.itemIndex).getAmount().isRange();
    }

    return false;
}

QString ListModelRecipeItems::additionalInfo(int row) const
{
    ItemInfo info;
    getItemInfo(row, info);

    if(info.type == ItemType::RegularItem)
    {
        return m_pRecipe->getRecipeItems().getItemAt(info.itemIndex).getAdditionalInfo();
    }
    else if(info.type == ItemType::GroupItem)
    {
        return info.pGroup->getItemAt(info.itemIndex).getAdditionalInfo();
    }

    return "";
}

quint32 ListModelRecipeItems::sizeIndex(int row) const
{
    ItemInfo info;
    getItemInfo(row, info);

    if(info.type == ItemType::RegularItem)
    {
        return (int)m_pRecipe->getRecipeItems().getItemAt(info.itemIndex).getSize();
    }
    else if(info.type == ItemType::GroupItem)
    {
        return (int)info.pGroup->getItemAt(info.itemIndex).getSize();
    }

    return 1;
}

bool ListModelRecipeItems::optional(int row) const
{
    ItemInfo info;
    getItemInfo(row, info);

    if(info.type == ItemType::RegularItem)
    {
        return m_pRecipe->getRecipeItems().getItemAt(info.itemIndex).isOptional();
    }
    else if(info.type == ItemType::GroupItem)
    {
        return info.pGroup->getItemAt(info.itemIndex).isOptional();
    }

    return false;
}
        
void ListModelRecipeItems::setAmountUnit(int row, quint32 uiUnit)
{
    ItemInfo info;
    getItemInfo(row, info);

    if(info.type == ItemType::RegularItem)
    {
        m_pRecipe->getRecipeItems().getItemAt(info.itemIndex).getAmount().setUnit((Unit)uiUnit);
        setDataChanged(row, RecipeItemsRoles::AmountUnitRole);
    }
    else if(info.type == ItemType::GroupItem)
    {
        info.pGroup->getItemAt(info.itemIndex).getAmount().setUnit((Unit)uiUnit);
        setDataChanged(row, RecipeItemsRoles::AmountUnitRole);
    }
}

void ListModelRecipeItems::setAmountMin(int row, float amount)
{
    ItemInfo info;
    getItemInfo(row, info);

    if(info.type == ItemType::RegularItem)
    {
        m_pRecipe->getRecipeItems().getItemAt(info.itemIndex).getAmount().setQuantityMin(amount);
        setDataChanged(row, RecipeItemsRoles::AmountMinRole);
    }
    else if(info.type == ItemType::GroupItem)
    {
        info.pGroup->getItemAt(info.itemIndex).getAmount().setQuantityMin(amount);
        setDataChanged(row, RecipeItemsRoles::AmountMinRole);
    }
}

void ListModelRecipeItems::setAmountMax(int row, float amount)
{
    ItemInfo info;
    getItemInfo(row, info);

    if(info.type == ItemType::RegularItem)
    {
        m_pRecipe->getRecipeItems().getItemAt(info.itemIndex).getAmount().setQuantityMax(amount);
        setDataChanged(row, RecipeItemsRoles::AmountMinRole);
    }
    else if(info.type == ItemType::GroupItem)
    {
        info.pGroup->getItemAt(info.itemIndex).getAmount().setQuantityMax(amount);
        setDataChanged(row, RecipeItemsRoles::AmountMaxRole);
    }
}

void ListModelRecipeItems::setAmountIsRange(int row, bool bRange)
{
    ItemInfo info;
    getItemInfo(row, info);

    if(info.type == ItemType::RegularItem)
    {
        m_pRecipe->getRecipeItems().getItemAt(info.itemIndex).getAmount().setIsRange(bRange);
        setDataChanged(row, RecipeItemsRoles::AmountIsRangeRole);
    }
    else if(info.type == ItemType::GroupItem)
    {
        info.pGroup->getItemAt(info.itemIndex).getAmount().setIsRange(bRange);
        setDataChanged(row, RecipeItemsRoles::AmountIsRangeRole);
    }
}

void ListModelRecipeItems::setAdditionalInfo(int row, QString text)
{
    ItemInfo info;
    getItemInfo(row, info);

    if(info.type == ItemType::RegularItem)
    {
        m_pRecipe->getRecipeItems().getItemAt(info.itemIndex).setAdditionInfo(text);
         setDataChanged(row, RecipeItemsRoles::AdditionalInfoRole);
    }
    else if(info.type == ItemType::GroupItem)
    {
        info.pGroup->getItemAt(info.itemIndex).setAdditionInfo(text);
        setDataChanged(row, RecipeItemsRoles::AdditionalInfoRole);
    }
}

void ListModelRecipeItems::setSizeIndex(int row, quint32 index)
{
    ItemInfo info;
    getItemInfo(row, info);

    if(info.type == ItemType::RegularItem)
    {
        m_pRecipe->getRecipeItems().getItemAt(info.itemIndex).setSize((Size)index);
        setDataChanged(row, RecipeItemsRoles::SizeRole);
    }
    else if(info.type == ItemType::GroupItem)
    {
        info.pGroup->getItemAt(info.itemIndex).setSize((Size)index);
        setDataChanged(row, RecipeItemsRoles::SizeRole);
    }
}

void ListModelRecipeItems::setOptional(int row, bool bOptional)
{
    ItemInfo info;
    getItemInfo(row, info);

    if(info.type == ItemType::RegularItem)
    {
        m_pRecipe->getRecipeItems().getItemAt(info.itemIndex).setIsOptional(bOptional);
        setDataChanged(row, RecipeItemsRoles::OptionalRole);
    }
    else if(info.type == ItemType::GroupItem)
    {
        info.pGroup->getItemAt(info.itemIndex).setIsOptional(bOptional);
        setDataChanged(row, RecipeItemsRoles::OptionalRole);
    }
}

int ListModelRecipeItems::addRecipeGroup(QString strName)
{
    if(m_pRecipe == nullptr || m_pRecipe->existsAlternativesGroup(strName))
    {
        return -1;
    }
    
    qint32 index = rowCount();

    beginInsertRows(QModelIndex(),index, index);

    m_pRecipe->addAlternativesGroup(strName);

    endInsertRows();

    return index;
}

bool ListModelRecipeItems::existsGroup(QString strGroup) const
{
    if(m_pRecipe == nullptr)
    {
        return false;
    }
    return m_pRecipe->existsAlternativesGroup(strGroup);
}

int ListModelRecipeItems::renameGroup(int row, QString newName)
{
    ItemInfo info;
    getItemInfo(row, info);

    if(info.type != ItemType::GroupHeader)
    {
        return -1;
    }

    if(m_pRecipe == nullptr || m_pRecipe->existsAlternativesGroup(newName))
    {
        return -1;
    }
    
    info.pGroup->rename(newName);

    dataChanged(index(row), index(row));

    return row;
}

bool ListModelRecipeItems::removeItem(int row)
{
    ItemInfo info;
    getItemInfo(row, info);

    if(info.type == ItemType::None)
    {
        return false;
    }

    RecipeItemGroup* pGroup = info.pGroup;
    if(info.type == ItemType::RegularItem)
    {
        pGroup = &m_pRecipe->getRecipeItems();
    }

    if(pGroup == nullptr)
    {
        return false;
    }

    bool bSuccess = false;
    if(info.type == ItemType::GroupHeader)
    {
        beginRemoveRows(QModelIndex(), row, row + pGroup->getItemsCount());
        m_pRecipe->removeAlternativesGroup(*pGroup);
        endRemoveRows();
    }
    else 
    {
        beginRemoveRows(QModelIndex(), row, row);
        pGroup->removeItem(pGroup->getItemAt(info.itemIndex));
        endRemoveRows();
    }

    return bSuccess;
}

void ListModelRecipeItems::onIngredientRenamed(quint32 row)
{
    if(m_pRecipe != nullptr)
    {
        dataChanged(index(0), index(rowCount()-1));
    }
}
