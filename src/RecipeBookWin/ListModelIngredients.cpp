#include "ListModelIngredients.h"
#include <QException>
#include <data/RecipeBook.h>
#include <data/Category.h>
#include <data/Ingredient.h>
#include <data/SortOrder.h>
#include <data/Recipe.h>
#include <data/ShoppingRecipe.h>
#include <data/RBDataHandler.h>
#include "uistringconverter.h"
#include "RecipeBookSettings.h"

using namespace recipebook::UI;

ListModelIngredients::ListModelIngredients(recipebook::RBDataHandler& rRBDataHandler, 
										   const recipebook::RecipeBookSettings& rSettings,
										   const UIStringConverter& rConverter)
:	m_rRBDataHandler(rRBDataHandler),
	m_rSettings(rSettings),
	m_rConverter(rConverter)
{
}

int ListModelIngredients::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);
	return handle.data().getIngredientsCount();
}

QVariant ListModelIngredients::data(const QModelIndex& index, int iRole) const
{
	IngredientRoles role = static_cast<IngredientRoles>(iRole);
	if(role == IngredientRoles::NameRole)
	{
		return name(index.row());
	}
	else if(role == IngredientRoles::CategoryRole)
	{
		return category(index.row());
	}
	else if(role == IngredientRoles::DefaultUnitRole)
	{
		return defaultUnit(index.row());
	}

	return QVariant();
}

QString ListModelIngredients::name(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getIngredientsCount())
		return "";

	const Ingredient& rIngredient = handle.data().getIngredientAt(row);
	return rIngredient.getName();
}

QString ListModelIngredients::category(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getIngredientsCount())
		return "";

	const Ingredient& rIngredient = handle.data().getIngredientAt(row);
	return rIngredient.getCategory().getName();
}

bool ListModelIngredients::provenanceAvailable(int row, QString strProvenance) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getIngredientsCount())
		return false;

	if(!handle.data().existsSortOrder(strProvenance))
		return false;

	const Ingredient& rIngredient = handle.data().getIngredientAt(row);
	const SortOrder& rOrder = handle.data().getSortOrder(strProvenance);
	
	return rIngredient.provenanceAvailable(rOrder);
}

QString ListModelIngredients::defaultUnit(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getIngredientsCount())
		return "";

	const Ingredient& rIngredient = handle.data().getIngredientAt(row);
	return  m_rConverter.convertUnit(rIngredient.getDefaultUnit());
}

QString ListModelIngredients::listUsedInRecipes(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getIngredientsCount())
		return " -";

	const Ingredient& rIngredient = handle.data().getIngredientAt(row);

	QList<Recipe*> recipes;
	QList<ShoppingRecipe*> shoppingRecipes;
	if(!handle.data().isIngredientInUse(rIngredient, &recipes, &shoppingRecipes))
	{
		return " -";
	}

	QString text;
	if(recipes.size() > 0)
	{
		text = "<ul>";
		for(Recipe* pRecipe : qAsConst(recipes))
		{
			text += "<li>" + pRecipe->getName() + "</li>";
		}
		text += "</ul>";
	}
	else
	{
		text = " -";
	}

	return text;
}

QString ListModelIngredients::listUsedInShoppingRecipes(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getIngredientsCount())
		return " -";

	const Ingredient& rIngredient = handle.data().getIngredientAt(row);

	QList<Recipe*> recipes;
	QList<ShoppingRecipe*> shoppingRecipes;
	if(!handle.data().isIngredientInUse(rIngredient, &recipes, &shoppingRecipes))
	{
		return " -";
	}

	QString text;
	if(shoppingRecipes.size() > 0)
	{
		text = "<ul>";
		for(ShoppingRecipe* pRecipe : qAsConst(shoppingRecipes))
		{
			text += "<li>" + pRecipe->getName() + "</li>";
		}
		text += "</ul>";
	}
	else
	{
		text = " -";
	}

	return text;
}

void ListModelIngredients::setCategory(int row, QString newCategory)
{
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getIngredientsCount())
			return;

		if(!handle.data().existsCategory(newCategory))
			return;

		const Category& rCategory = handle.data().getCategory(newCategory);
		Ingredient& rIngredient = handle.data().getIngredientAt(row);
		rIngredient.setCategory(rCategory);
	}

	setDataChanged(row, IngredientRoles::CategoryRole);
}

void ListModelIngredients::setProvenanceAvailable(int row, QString strProvenance, bool bAvailable)
{
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getIngredientsCount())
			return;

		Ingredient& rIngredient = handle.data().getIngredientAt(row);
		if(!handle.data().existsSortOrder(strProvenance))
		{
			return;
		}

		const SortOrder& rSortOrder = handle.data().getSortOrder(strProvenance);
		if(bAvailable)
		{
			rIngredient.setProvenanceAvailable(rSortOrder);
		}
		else
		{
			rIngredient.setProvenanceUnavailable(rSortOrder);
		}
	}
}

void ListModelIngredients::setDefaultUnit(int row, QString newDefaultUnit)
{
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getIngredientsCount())
			return;

		Unit unit = m_rConverter.convertUnit(newDefaultUnit);
		Ingredient& rIngredient = handle.data().getIngredientAt(row);
		rIngredient.setDefaultUnit(unit);
	}

	setDataChanged(row, IngredientRoles::DefaultUnitRole);
}

QHash<int, QByteArray> ListModelIngredients::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[(int)IngredientRoles::NameRole] = "name";
	roles[(int)IngredientRoles::CategoryRole] = "category";
	roles[(int)IngredientRoles::DefaultUnitRole] = "defaultUnit";
	return roles;
}

void ListModelIngredients::setDataChanged(int row, IngredientRoles role)
{
	QVector<int> rolesChanged;
	rolesChanged.append((int)role);
	dataChanged(index(row), index(row), rolesChanged);
}

int ListModelIngredients::renameIngredient(int row, QString newName)
{
	qint32 newIndex = -1;
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getIngredientsCount())
			return -1;

		if(handle.data().existsIngredient(newName))
		{
			return -1;
		}

		newIndex = handle.data().getIngredientIndex(newName);
	}

	if(row != newIndex)
	{
		beginMoveRows(QModelIndex(), row, row, QModelIndex(), newIndex);
	}

	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

		if(newIndex > row)
		{
			newIndex -= 1;
		}

		Ingredient& rIngredient = handle.data().getIngredientAt(row);
		handle.data().renameIngredient(rIngredient, newName);
	}

	if(row != newIndex)
	{
		endMoveRows();
	}

	dataChanged(index(newIndex), index(newIndex));

	emit ingredientRenamed(row);

	return newIndex;
}

int ListModelIngredients::addIngredient(QString strIngredient)
{
	qint32 index = -1;
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);

		if(handle.data().existsIngredient(strIngredient))
		{
			return -1;
		}

		if(handle.data().getCategoriesCount() == 0)
		{
			return -1;
		}

		index = handle.data().getIngredientIndex(strIngredient);
	}

	beginInsertRows(QModelIndex(), index, index);

	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);
		const Category* pCategory = &handle.data().getCategoryAt(0);
		QString defaultCategory = m_rSettings.getDefaultCategory();
		if(!defaultCategory.isEmpty() && handle.data().existsCategory(defaultCategory))
		{
			pCategory = &handle.data().getCategory(defaultCategory);
		}
		handle.data().addIngredient(strIngredient, *pCategory, (Unit) m_rSettings.getDefaultUnit());
	}

	endInsertRows();

	return index;
}

bool ListModelIngredients::existsIngredient(QString strIngredient) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);
	return handle.data().existsIngredient(strIngredient);
}

int ListModelIngredients::indexOfIngredient(QString strIngredient) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);
	return handle.data().getIngredientIndex(strIngredient);
}

bool ListModelIngredients::canIngredientBeRemoved(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getIngredientsCount())
	{
		return false;
	}

	const Ingredient& rIngredient = handle.data().getIngredientAt(row);
	return !handle.data().isIngredientInUse(rIngredient);
}

bool ListModelIngredients::removeIngredient(int row)
{
	bool bSuccess = false;
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getIngredientsCount())
		{
			return false;
		}

		if(!canIngredientBeRemoved(row))
		{
			return false;
		}
	}

	beginRemoveRows(QModelIndex(), row, row);

	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);
		Ingredient& rIngredient = handle.data().getIngredientAt(row);
		bSuccess = handle.data().removeIngredient(rIngredient);
	}

	endRemoveRows();

	return bSuccess;
}

void ListModelIngredients::onCategoryRenamed(quint32 row)
{
	Q_UNUSED(row);

	int ingredientsCount = 0;
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);
		ingredientsCount = handle.data().getIngredientsCount();
	}
	dataChanged(index(0), index(ingredientsCount-1));
}

void ListModelIngredients::onSortOrderRenamed(quint32 row)
{
	Q_UNUSED(row);

	int ingredientsCount = 0;
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);
		ingredientsCount = handle.data().getIngredientsCount();
	}
	dataChanged(index(0), index(ingredientsCount-1));
}

void ListModelIngredients::onDataReset()
{
	beginResetModel();
	endResetModel();
}
