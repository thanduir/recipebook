#include "ListModelProvenance.h"
#include <data/RecipeBook.h>
#include <data/Ingredient.h>
#include <data/SortOrder.h>
#include "uistringconverter.h"

using namespace recipebook::UI;

const quint32 c_uiRowEverywhere = 0;

ListModelProvenance::ListModelProvenance(recipebook::RecipeBook& rRecipeBook, const UIStringConverter& rConverter)
:	m_rRecipeBook(rRecipeBook),
    m_rConverter(rConverter)
{
}

int ListModelProvenance::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
    return m_rRecipeBook.getSortOrdersCount() + 1;
}

QVariant ListModelProvenance::data(const QModelIndex& index, int iRole) const
{
	if (index.row() < 0 || index.row() >= (int)m_rRecipeBook.getIngredientsCount())
        return QVariant();

    if(index.row() == c_uiRowEverywhere)
    {
        return m_rConverter.getProvenanceEverywhere();
    }

    const SortOrder& rSortOrder = m_rRecipeBook.getSortOrderAt(index.row()-1);
    ProvenanceRoles role = static_cast<ProvenanceRoles>(iRole);
    if(role == ProvenanceRoles::NameRole)
    {
        return rSortOrder.getName();
    }

    return QVariant();
}

QHash<int, QByteArray> ListModelProvenance::roleNames() const
{
	QHash<int, QByteArray> roles;
    roles[(int)ProvenanceRoles::NameRole] = "name";
    return roles;
}

QString ListModelProvenance::provenanceEverywhere() const
{
    return m_rConverter.getProvenanceEverywhere();
}

QString ListModelProvenance::name(int row) const
{
    if(row < 0 || row >= (int) m_rRecipeBook.getSortOrdersCount() + 1)
        return "";

    if(row == c_uiRowEverywhere)
    {
        return provenanceEverywhere();
    }

    SortOrder& rSortOrder = m_rRecipeBook.getSortOrderAt(row - 1);
    return rSortOrder.getName();
}

int ListModelProvenance::renameProvenance(int row, QString newName)
{
    // Provenance "Everywhere" can't be renamed.
    if(row <= 0 || row >= (int) m_rRecipeBook.getSortOrdersCount() + 1)
        return -1;

    if(m_rRecipeBook.existsSortOrder(newName))
    {
        return -1;
    }

    qint32 newIndex = m_rRecipeBook.getSortOrderIndex(newName) + 1;
    if(row != newIndex)
    {
        beginMoveRows(QModelIndex(), row, row, QModelIndex(), newIndex);
    }

    if(newIndex > row)
    {
        newIndex -= 1;
    }

    SortOrder& rSortOrder = m_rRecipeBook.getSortOrderAt(row - 1);
    m_rRecipeBook.renameSortOrder(rSortOrder, newName);

    if(row != newIndex)
    {
        endMoveRows();
    }

    dataChanged(index(newIndex), index(newIndex));

    emit provenanceRenamed(newIndex);

    return newIndex;
}

int ListModelProvenance::addProvenance(QString strSortOrder)
{
    if(m_rRecipeBook.existsSortOrder(strSortOrder) || strSortOrder == provenanceEverywhere())
    {
        return -1;
    }

    qint32 index = m_rRecipeBook.getSortOrderIndex(strSortOrder) + 1;

    beginInsertRows(QModelIndex(), index, index);
    m_rRecipeBook.addSortOrder(strSortOrder);
    endInsertRows();

    return index;
}

bool ListModelProvenance::existsProvenance(QString strSortOrder) const
{
    if(strSortOrder == provenanceEverywhere())
    {
        return true;
    }

    return m_rRecipeBook.existsSortOrder(strSortOrder);
}

QString ListModelProvenance::listUsedInIngredients(int row) const
{
    // Not implemented for provenance "Everywhere".
    if(row <= 0 || row >= (int) m_rRecipeBook.getSortOrdersCount() + 1)
        return " -";

    SortOrder& rSortOrder = m_rRecipeBook.getSortOrderAt(row - 1);

    QList<Ingredient*> ingredients;
    if(!m_rRecipeBook.isSortOrderInUse(rSortOrder, &ingredients))
    {
        return " -";
    }

    QString text;
    if(ingredients.size() > 0)
    {
        text = "<ul>";
        for(Ingredient* pIngredient : qAsConst(ingredients))
        {
            text += "<li>" + pIngredient->getName() + "</li>";
        }
        text += "</ul>";
    }
    else
    {
        text = " -";
    }

    return text;
}

bool ListModelProvenance::canProvenanceBeRemoved(int row) const
{
    if(row == c_uiRowEverywhere || row < 0 || row >= (int) m_rRecipeBook.getSortOrdersCount() + 1)
    {
        return false;
    }

    SortOrder& rSortOrder = m_rRecipeBook.getSortOrderAt(row - 1);
    return !m_rRecipeBook.isSortOrderInUse(rSortOrder);
}

bool ListModelProvenance::removeProvenance(int row)
{
    if(row < 0 || row >= (int) m_rRecipeBook.getSortOrdersCount() + 1)
    {
        return false;
    }

    if(!canProvenanceBeRemoved(row))
    {
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row);

    SortOrder& rSortOrder = m_rRecipeBook.getSortOrderAt(row - 1);
    bool bSuccess = m_rRecipeBook.removeSortOrder(rSortOrder);

    endRemoveRows();

    return bSuccess;
}

void ListModelProvenance::onDataReset()
{
    beginResetModel();
    endResetModel();
}
