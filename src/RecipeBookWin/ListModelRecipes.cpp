#include "ListModelRecipes.h"
#include <QException>
#include <data/RecipeBook.h>
#include <data/Recipe.h>
#include "RBDataHandler.h"
#include "RecipeBookSettings.h"

using namespace recipebook::UI;

ListModelRecipes::ListModelRecipes(recipebook::RBDataHandler& rRBDataHandler, const recipebook::RecipeBookSettings& rSettings)
:	m_rRBDataHandler(rRBDataHandler),
	m_rSettings(rSettings)
{
}

int ListModelRecipes::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);
	return handle.data().getRecipesCount();
}

QVariant ListModelRecipes::data(const QModelIndex& index, int iRole) const
{
	RecipeRoles role = static_cast<RecipeRoles>(iRole);
	if(role == RecipeRoles::NameRole)
	{
		return name(index.row());
	}
	else if(role == RecipeRoles::NumberOfPersons)
	{
		return numberOfPersons(index.row());
	}
	else if(role == RecipeRoles::ShortDescription)
	{
		return shortDescription(index.row());
	}
	else if(role == RecipeRoles::RecipeText)
	{
		return recipeText(index.row());
	}
	else if(role == RecipeRoles::CookingTimeInMin)
	{
		return cookingTime(index.row());
	}
	else if(role == RecipeRoles::EverythingSet)
	{
		return isEverythingSet(index.row());
	}

	return QVariant();
}

QHash<int, QByteArray> ListModelRecipes::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[(int)RecipeRoles::NameRole] = "name";
	roles[(int)RecipeRoles::NumberOfPersons] = "numberOfPersons";
	roles[(int)RecipeRoles::ShortDescription] = "shortDesc";
	roles[(int)RecipeRoles::RecipeText] = "recipeText";
	roles[(int)RecipeRoles::CookingTimeInMin] = "cookingTime";
	roles[(int)RecipeRoles::EverythingSet] = "isEverythingSet";
	return roles;
}

void ListModelRecipes::setDataChanged(int row, RecipeRoles role)
{
	QVector<int> rolesChanged;
	rolesChanged.append((int)role);
	dataChanged(index(row), index(row), rolesChanged);
}

QString ListModelRecipes::name(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getRecipesCount())
		return "";

	const Recipe& rRecipe = handle.data().getRecipeAt(row);
	return rRecipe.getName();
}

bool ListModelRecipes::isEverythingSet(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getRecipesCount())
		return false;

	const Recipe& rRecipe = handle.data().getRecipeAt(row);
    
	return rRecipe.getRecipeText() != "" 
			&& rRecipe.getShortDescription() != "" 
			&& rRecipe.getNumberOfPersons() > 0 
			&& !rRecipe.getCookingTime().isNull()
			&& rRecipe.getRecipeItemsCount() > 0;
}

quint32 ListModelRecipes::numberOfPersons(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getRecipesCount())
		return 0;

	const Recipe& rRecipe = handle.data().getRecipeAt(row);
	return rRecipe.getNumberOfPersons();
}

QString ListModelRecipes::shortDescription(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getRecipesCount())
		return 0;

	const Recipe& rRecipe = handle.data().getRecipeAt(row);
	return rRecipe.getShortDescription();
}

QString ListModelRecipes::recipeText(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getRecipesCount())
		return 0;

	const Recipe& rRecipe = handle.data().getRecipeAt(row);
	return rRecipe.getRecipeText();
}

quint32 ListModelRecipes::cookingTime(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getRecipesCount())
		return 0;

	const Recipe& rRecipe = handle.data().getRecipeAt(row);
	QTime time = rRecipe.getCookingTime();
	return time.minute() + 60 * time.hour();
}

void ListModelRecipes::setNumberOfPersons(int row, qint32 nrPersons)
{
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getRecipesCount())
			return;

		Recipe& rRecipe = handle.data().getRecipeAt(row);
		rRecipe.setNumberOfPersons(nrPersons);
	}

	setDataChanged(row, RecipeRoles::NumberOfPersons);
	setDataChanged(row, RecipeRoles::EverythingSet);
}

void ListModelRecipes::setShortDescription(int row, QString desc)
{
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getRecipesCount())
			return;

		Recipe& rRecipe = handle.data().getRecipeAt(row);
		rRecipe.setShortDescription(desc);
	}

	setDataChanged(row, RecipeRoles::ShortDescription);
	setDataChanged(row, RecipeRoles::EverythingSet);
}

void ListModelRecipes::setRecipeText(int row, QString text)
{
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getRecipesCount())
			return;

		Recipe& rRecipe = handle.data().getRecipeAt(row);
		rRecipe.setRecipeText(text);
	}

	setDataChanged(row, RecipeRoles::RecipeText);
	setDataChanged(row, RecipeRoles::EverythingSet);
}

void ListModelRecipes::setCookingTime(int row, quint32 timeInMin)
{
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getRecipesCount())
			return;

		Recipe& rRecipe = handle.data().getRecipeAt(row);
		QTime time(timeInMin / 60, timeInMin % 60);
		rRecipe.setCookingTime(time);
	}

	setDataChanged(row, RecipeRoles::CookingTimeInMin);
	setDataChanged(row, RecipeRoles::EverythingSet);
}

int ListModelRecipes::renameRecipe(int row, QString newName)
{
	qint32 newIndex = -1;
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getRecipesCount())
			return -1;

		if(handle.data().existsRecipe(newName))
		{
			return -1;
		}

		newIndex = handle.data().getRecipeIndex(newName);
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

		Recipe& rRecipe = handle.data().getRecipeAt(row);
		handle.data().renameRecipe(rRecipe, newName);
	}

	if(row != newIndex)
	{
		endMoveRows();
	}

	dataChanged(index(newIndex), index(newIndex));

	return newIndex;
}

int ListModelRecipes::addRecipe(QString strRecipe)
{
	qint32 index = -1;
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);

		if(handle.data().existsRecipe(strRecipe))
		{
			return -1;
		}

		index = handle.data().getRecipeIndex(strRecipe);
	}

	beginInsertRows(QModelIndex(), index, index);

	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);
		handle.data().addRecipe(strRecipe, m_rSettings.getDefaultRecipeNrPersons());
	}

	endInsertRows();

	return index;
}

int ListModelRecipes::copyRecipe(int row, QString strRecipe)
{
	qint32 index = -1;
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getRecipesCount())
			return -1;

		if(handle.data().existsRecipe(strRecipe))
		{
			return -1;
		}

		index = handle.data().getRecipeIndex(strRecipe);
	}

	beginInsertRows(QModelIndex(),index, index);

	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);
		const Recipe& rRecipe = handle.data().getRecipeAt(row);
		handle.data().copyRecipe(rRecipe, strRecipe);
	}

	endInsertRows();

	return index;
}

bool ListModelRecipes::existsRecipe(QString strRecipe) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);
	return handle.data().existsRecipe(strRecipe);
}

bool ListModelRecipes::removeRecipe(int row)
{
	bool bSuccess = false;
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getRecipesCount())
		{
			return false;
		}
	}

	beginRemoveRows(QModelIndex(), row, row);

	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);
		Recipe& rRecipe = handle.data().getRecipeAt(row);
		bSuccess = handle.data().removeRecipe(rRecipe);
	}

	endRemoveRows();

	return bSuccess;
}

void ListModelRecipes::onDataReset()
{
	beginResetModel();
	endResetModel();
}
