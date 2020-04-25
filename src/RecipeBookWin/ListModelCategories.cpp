#include "ListModelCategories.h"
#include <data/RecipeBook.h>
#include <data/Category.h>
#include <data/Ingredient.h>

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

QString ListModelCategories::name(int row) const
{
	if(row < 0 || row >= (int)m_rRecipeBook.getCategoriesCount())
		return "";

	const Category& rCategory = m_rRecipeBook.getCategoryAt(row);
	return rCategory.getName();
}

int ListModelCategories::renameCategory(int row, QString newName)
{
	if(row < 0 || row >= (int)m_rRecipeBook.getCategoriesCount())
		return -1;

	if(m_rRecipeBook.existsCategory(newName))
	{
		return -1;
	}

	Category& rCategory = m_rRecipeBook.getCategoryAt(row);
	m_rRecipeBook.renameCategory(rCategory, newName);

	dataChanged(index(row), index(row));
    
	emit categoryRenamed(row);

	return row;
}

int ListModelCategories::addCategory(QString strCategory)
{
	if(m_rRecipeBook.existsCategory(strCategory))
	{
		return -1;
	}

	qint32 index = m_rRecipeBook.getCategoryIndex(strCategory);

	beginInsertRows(QModelIndex(),index, index);
	m_rRecipeBook.addCategory(strCategory);
	endInsertRows();

	return index;
}

bool ListModelCategories::existsCategory(QString strCategory) const
{
	return m_rRecipeBook.existsCategory(strCategory);
}

QString ListModelCategories::listUsedInIngredients(int row) const
{
	if(row < 0 || row >= (int)m_rRecipeBook.getCategoriesCount())
		return " -";

	const Category& rCategory = m_rRecipeBook.getCategoryAt(row);

	QList<Ingredient*> ingredients;
	if(!m_rRecipeBook.isCategoryInUse(rCategory, &ingredients))
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

bool ListModelCategories::canCategoryBeRemoved(int row) const
{
	if(row < 0 || row >= (int)m_rRecipeBook.getCategoriesCount())
	{
		return false;
	}

	const Category& rCategory = m_rRecipeBook.getCategoryAt(row);
	return !m_rRecipeBook.isCategoryInUse(rCategory);
}

bool ListModelCategories::removeCategory(int row)
{
	if(row < 0 || row >= (int)m_rRecipeBook.getCategoriesCount())
	{
		return false;
	}

	if(!canCategoryBeRemoved(row))
	{
		return false;
	}

	beginRemoveRows(QModelIndex(), row, row);

	const Category& rCategory = m_rRecipeBook.getCategoryAt(row);
	bool bSuccess = m_rRecipeBook.removeCategory(rCategory);

	endRemoveRows();

	return bSuccess;
}

void ListModelCategories::onDataReset()
{
	beginResetModel();
	endResetModel();
}
