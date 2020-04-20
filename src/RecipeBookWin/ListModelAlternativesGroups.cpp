#include "ListModelAlternativesGroups.h"
#include <data/RecipeBook.h>
#include <data/AlternativesType.h>
#include "uistringconverter.h"

using namespace recipebook::UI;

const quint32 c_uiRowNoGroup = 0;

ListModelAlternativesGroups::ListModelAlternativesGroups(recipebook::RecipeBook& rRecipeBook, const UIStringConverter& rConverter)
:	m_rRecipeBook(rRecipeBook),
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
    return m_rRecipeBook.getAlternativesTypesCount() + 1;
}

QVariant ListModelAlternativesGroups::data(const QModelIndex& index, int iRole) const
{
	if (index.row() < 0 || index.row() >= (int)m_rRecipeBook.getAlternativesTypesCount() + 1)
        return QVariant();

    if(index.row() == c_uiRowNoGroup)
    {
        return stringNoAlternativesGroup();
    }

    const AlternativesType& rType = m_rRecipeBook.getAlternatiesTypeAt(index.row() - 1);
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
    if(row < 0 || row >= (int)m_rRecipeBook.getAlternativesTypesCount() + 1)
        return "";

    if(row == c_uiRowNoGroup)
    {
        return stringNoAlternativesGroup();
    }

    const AlternativesType& rType = m_rRecipeBook.getAlternatiesTypeAt(row - 1);
    return rType.getName();
}

int ListModelAlternativesGroups::renameType(int row, QString newName)
{
    if(row == c_uiRowNoGroup || row < 0 || row >= (int)m_rRecipeBook.getAlternativesTypesCount() + 1)
        return -1;

    if(m_rRecipeBook.existsAlternativesType(newName))
    {
        return -1;
    }

    AlternativesType& rType = m_rRecipeBook.getAlternatiesTypeAt(row - 1);
    m_rRecipeBook.renameAlternativesType(rType, newName);

    QVector<int> rolesChanged;
    rolesChanged.append((int)AlternativesGroupsRoles::NameRole);
    dataChanged(index(row), index(row), rolesChanged);
    
    emit alternativesGroupChanged(row);

    return row;
}

QString ListModelAlternativesGroups::color(int row) const
{
    if(row == c_uiRowNoGroup || row < 0 || row >= (int)m_rRecipeBook.getAlternativesTypesCount() + 1)
        return "";

    const AlternativesType& rType = m_rRecipeBook.getAlternatiesTypeAt(row - 1);
    return rType.getColor().name();
}

void ListModelAlternativesGroups::setColor(int row, QString strColor)
{
    if(row < 0 || row >= (int) m_rRecipeBook.getIngredientsCount())
        return;

    AlternativesType& rType = m_rRecipeBook.getAlternatiesTypeAt(row - 1);
    rType.setColor(strColor);

    QVector<int> rolesChanged;
    rolesChanged.append((int)AlternativesGroupsRoles::ColorRole);
    dataChanged(index(row), index(row), rolesChanged);

    emit alternativesGroupChanged(row);
}

int ListModelAlternativesGroups::addType(QString strType)
{
    if(m_rRecipeBook.existsAlternativesType(strType) || strType == stringNoAlternativesGroup())
    {
        return -1;
    }

    qint32 index = m_rRecipeBook.getAlternativesTypeIndex(strType) + 1;

    beginInsertRows(QModelIndex(),index, index);
    m_rRecipeBook.addAlternativesType(strType);
    endInsertRows();

    return index;
}

bool ListModelAlternativesGroups::existsType(QString strType) const
{
    if(strType == stringNoAlternativesGroup())
    {
        return true;
    }

    return m_rRecipeBook.existsAlternativesType(strType);
}

bool ListModelAlternativesGroups::canTypeBeRemoved(int row) const
{
    if(row == c_uiRowNoGroup || row < 0 || row >= (int)m_rRecipeBook.getAlternativesTypesCount() + 1)
    {
        return false;
    }

    const AlternativesType& rType = m_rRecipeBook.getAlternatiesTypeAt(row - 1);
    return !m_rRecipeBook.isAlternativesTypeInUse(rType);
}

bool ListModelAlternativesGroups::removeType(int row)
{
    if(row == c_uiRowNoGroup || row < 0 || row >= (int)m_rRecipeBook.getAlternativesTypesCount() + 1)
    {
        return false;
    }

    if(!canTypeBeRemoved(row))
    {
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row);

    const AlternativesType& rType = m_rRecipeBook.getAlternatiesTypeAt(row - 1);
    bool bSuccess = m_rRecipeBook.removeAlternativesType(rType);

    endRemoveRows();

    return bSuccess;
}

int ListModelAlternativesGroups::indexOf(QString strName) const
{
    if(strName == m_rConverter.getStringNoAlternativesGroup())
    {
        return 0;
    }

    if(!m_rRecipeBook.existsAlternativesType(strName))
    {
        return -1;
    }

    return m_rRecipeBook.getAlternativesTypeIndex(strName) + 1;
}
