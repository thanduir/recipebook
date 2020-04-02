#include "ListModelCategories.h"
#include <data/RecipeBook.h>
#include <data/Category.h>

using namespace recipebook::UI;

ListModelCategories::ListModelCategories(recipebook::RecipeBook& rRecipeBook)
:	m_rRecipeBook(rRecipeBook)
{
}

int ListModelCategories::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
    return m_rRecipeBook.getCategoriesCount();
}

QVariant ListModelCategories::data(const QModelIndex& index, int iRole) const
{
	if (index.row() < 0 || index.row() >= (int)m_rRecipeBook.getCategoriesCount())
        return QVariant();

    const Category& rCategory = m_rRecipeBook.getCategoryAt(index.row());
    CategoryRoles role = static_cast<CategoryRoles>(iRole);
    if(role == CategoryRoles::NameRole)
    {
        return rCategory.getName();
    }

    return QVariant();
}

QHash<int, QByteArray> ListModelCategories::roleNames() const
{
	QHash<int, QByteArray> roles;
    roles[(int)CategoryRoles::NameRole] = "name";
    return roles;
}
