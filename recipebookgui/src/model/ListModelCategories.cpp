#include "ListModelCategories.h"
#include <data/RecipeBook.h>
#include <data/Category.h>
#include <data/Ingredient.h>
#include <data/RBDataHandler.h>

using namespace recipebook::UI;

ListModelCategories::ListModelCategories(recipebook::RBDataHandler& rRBDataHandler)
:	m_rRBDataHandler(rRBDataHandler)
{
}

int ListModelCategories::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);
    return (int)handle.data().getCategoriesCount();
}

QVariant ListModelCategories::data(const QModelIndex& index, int iRole) const
{
	CategoryRoles role = static_cast<CategoryRoles>(iRole);
	if(role == CategoryRoles::NameRole)
	{
		return name(index.row());
	}

	return QVariant();
}

QHash<int, QByteArray> ListModelCategories::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[(int)CategoryRoles::NameRole] = "name";
	return roles;
}

QString ListModelCategories::name(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int)handle.data().getCategoriesCount())
		return "";

    const Category& rCategory = handle.data().getCategoryAt((quint32)row);
	return rCategory.getName();
}

int ListModelCategories::renameCategory(int row, QString newName)
{
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getCategoriesCount())
			return -1;

		if(handle.data().existsCategory(newName))
		{
			return -1;
		}

        Category& rCategory = handle.data().getCategoryAt((quint32)row);
		handle.data().renameCategory(rCategory, newName);
	}

	dataChanged(index(row), index(row));
    
    emit categoryRenamed((quint32)row);

	return row;
}

bool ListModelCategories::canCategoriesBeAdded() const
{
	return true;
}

int ListModelCategories::addCategory(QString strCategory)
{
	if(!canCategoriesBeAdded())
	{
		return -1;
	}

	qint32 index = -1;
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);

		if(handle.data().existsCategory(strCategory))
		{
			return -1;
		}

        index = (qint32)handle.data().getCategoryIndex(strCategory);
	}

	beginInsertRows(QModelIndex(), index, index);

	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);
		handle.data().addCategory(strCategory);
	}

	endInsertRows();

	return index;
}

bool ListModelCategories::existsCategory(QString strCategory) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);
	return handle.data().existsCategory(strCategory);
}

bool ListModelCategories::canCategoryBeRemoved(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int)handle.data().getCategoriesCount())
	{
		return false;
	}

    const Category& rCategory = handle.data().getCategoryAt((quint32)row);
	return !handle.data().isCategoryInUse(rCategory);
}

bool ListModelCategories::removeCategory(int row)
{
	bool bSuccess = false;
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getCategoriesCount())
		{
			return false;
		}

		if(!canCategoryBeRemoved(row))
		{
			return false;
		}
	}

	beginRemoveRows(QModelIndex(), row, row);

	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);
        const Category& rCategory = handle.data().getCategoryAt((quint32)row);
		bSuccess = handle.data().removeCategory(rCategory);
	}

	endRemoveRows();

	return bSuccess;
}

void ListModelCategories::onDataReset()
{
	beginResetModel();
	endResetModel();
}
