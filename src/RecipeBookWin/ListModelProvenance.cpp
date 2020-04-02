#include "ListModelProvenance.h"
#include <data/RecipeBook.h>
#include <data/SortOrder.h>
#include "uistringconverter.h"

using namespace recipebook::UI;

ListModelProvenance::ListModelProvenance(recipebook::RecipeBook& rRecipeBook, const UIStringConverter& rConverter)
:	ListModelSortOrders(rRecipeBook),
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

    if(index.row() == 0)
    {
        return m_rConverter.getProvenanceEverywhere();
    }

    const SortOrder& rSortOrder = m_rRecipeBook.getSortOrderAt(index.row()-1);
    SortOrderRoles role = static_cast<SortOrderRoles>(iRole);
    if(role == SortOrderRoles::NameRole)
    {
        return rSortOrder.getName();
    }

    return QVariant();
}

QString ListModelProvenance::provenanceEverywhere() const
{
    return m_rConverter.getProvenanceEverywhere();
}
