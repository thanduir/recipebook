#include "ListModelSortOrders.h"
#include <data/RecipeBook.h>
//#include <data/Category.h>
#include <data/SortOrder.h>
//#include "uistringconverter.h"

using namespace recipebook::UI;

ListModelSortOrders::ListModelSortOrders(recipebook::RecipeBook& rRecipeBook)
:	m_rRecipeBook(rRecipeBook)
{
}

int ListModelSortOrders::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
    return m_rRecipeBook.getSortOrdersCount();
}

QVariant ListModelSortOrders::data(const QModelIndex& index, int iRole) const
{
	if (index.row() < 0 || index.row() >= (int)m_rRecipeBook.getIngredientsCount())
        return QVariant();

    const SortOrder& rSortOrder = m_rRecipeBook.getSortOrderAt(index.row());
    SortOrderRoles role = static_cast<SortOrderRoles>(iRole);
    if(role == SortOrderRoles::NameRole)
    {
        return rSortOrder.getName();
    }
    /*else if(role == IngredientRoles::CategoryRole)
    {
        return rIngredient.getCategory().getName();
    }
    else if(role == IngredientRoles::ProvenanceRole)
    {
        if(rIngredient.hasProvenanceEverywhere())
        {
            return m_rConverter.getProvenanceEverywhere();
        }
        else
        {
            return rIngredient.getProvenance().getName();
        }
    }
    else if(role == IngredientRoles::DefaultUnitRole)
    {
        return m_rConverter.convertUnit(rIngredient.getDefaultUnit());
    }*/

    return QVariant();
}

QHash<int, QByteArray> ListModelSortOrders::roleNames() const
{
	QHash<int, QByteArray> roles;
    roles[(int)SortOrderRoles::NameRole] = "name";
    /*roles[(int)IngredientRoles::CategoryRole] = "category";
	roles[(int)IngredientRoles::ProvenanceRole] = "provenance";
	roles[(int)IngredientRoles::DefaultUnitRole] = "defaultUnit";*/
    return roles;
}
