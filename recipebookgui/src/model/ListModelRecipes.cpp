#include "ListModelRecipes.h"
#include <QException>
#include <data/RecipeBook.h>
#include <data/Recipe.h>
#include <data/RBDataHandler.h>
#include "../RecipeBookSettings.h"

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
    return (int)handle.data().getRecipesCount();
}

QVariant ListModelRecipes::data(const QModelIndex& index, int iRole) const
{
	RecipeRoles role = static_cast<RecipeRoles>(iRole);
	if(role == RecipeRoles::NameRole)
	{
		return name(index.row());
	}
	else if(role == RecipeRoles::NumberOfPersonsRole)
	{
		return numberOfPersons(index.row());
	}
	else if(role == RecipeRoles::ShortDescriptionRole)
	{
		return shortDescription(index.row());
	}
	else if(role == RecipeRoles::RecipeTextRole)
	{
		return recipeText(index.row());
	}
	else if(role == RecipeRoles::CookingTimeInMinRole)
	{
		return cookingTime(index.row());
	}
	else if(role == RecipeRoles::EverythingSetRole)
	{
		return isEverythingSet(index.row());
	}

	return QVariant();
}

QHash<int, QByteArray> ListModelRecipes::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[(int)RecipeRoles::NameRole] = "name";
	roles[(int)RecipeRoles::NumberOfPersonsRole] = "numberOfPersons";
	roles[(int)RecipeRoles::ShortDescriptionRole] = "shortDesc";
	roles[(int)RecipeRoles::RecipeTextRole] = "recipeText";
	roles[(int)RecipeRoles::CookingTimeInMinRole] = "cookingTime";
	roles[(int)RecipeRoles::EverythingSetRole] = "isEverythingSet";
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

    const Recipe& rRecipe = handle.data().getRecipeAt((quint32)row);
	return rRecipe.getName();
}

bool ListModelRecipes::isEverythingSet(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getRecipesCount())
		return false;

    const Recipe& rRecipe = handle.data().getRecipeAt((quint32)row);
    
	return rRecipe.getRecipeText() != "" 
			&& rRecipe.getNumberOfPersons() > 0 
			&& !rRecipe.getCookingTime().isNull()
			&& rRecipe.getRecipeItemsCount() > 0;
}

quint32 ListModelRecipes::numberOfPersons(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getRecipesCount())
		return 0;

    const Recipe& rRecipe = handle.data().getRecipeAt((quint32)row);
	return rRecipe.getNumberOfPersons();
}

QString ListModelRecipes::shortDescription(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getRecipesCount())
		return 0;

    const Recipe& rRecipe = handle.data().getRecipeAt((quint32)row);
	return rRecipe.getShortDescription();
}

QString ListModelRecipes::recipeText(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getRecipesCount())
		return 0;

    const Recipe& rRecipe = handle.data().getRecipeAt((quint32)row);
	return rRecipe.getRecipeText();
}

quint32 ListModelRecipes::cookingTime(int row) const
{
	recipebook::RBDataReadHandle handle(m_rRBDataHandler);

	if(row < 0 || row >= (int) handle.data().getRecipesCount())
		return 0;

    const Recipe& rRecipe = handle.data().getRecipeAt((quint32)row);
	QTime time = rRecipe.getCookingTime();
    return static_cast<quint32>(time.minute() + 60 * time.hour());
}

void ListModelRecipes::setNumberOfPersons(int row, qint32 nrPersons)
{
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getRecipesCount())
			return;

        Recipe& rRecipe = handle.data().getRecipeAt((quint32)row);
        rRecipe.setNumberOfPersons((quint32)nrPersons);
	}

	setDataChanged(row, RecipeRoles::NumberOfPersonsRole);
	setDataChanged(row, RecipeRoles::EverythingSetRole);
}

void ListModelRecipes::setShortDescription(int row, QString desc)
{
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getRecipesCount())
			return;

        Recipe& rRecipe = handle.data().getRecipeAt((quint32)row);
		rRecipe.setShortDescription(desc);
	}

	setDataChanged(row, RecipeRoles::ShortDescriptionRole);
	setDataChanged(row, RecipeRoles::EverythingSetRole);
}

void ListModelRecipes::setRecipeText(int row, QString text)
{
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getRecipesCount())
			return;

        Recipe& rRecipe = handle.data().getRecipeAt((quint32)row);
		rRecipe.setRecipeText(text);
	}

	setDataChanged(row, RecipeRoles::RecipeTextRole);
	setDataChanged(row, RecipeRoles::EverythingSetRole);
}

void ListModelRecipes::setCookingTime(int row, quint32 timeInMin)
{
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);

		if(row < 0 || row >= (int) handle.data().getRecipesCount())
			return;

        Recipe& rRecipe = handle.data().getRecipeAt((quint32)row);
		QTime time(timeInMin / 60, timeInMin % 60);
		rRecipe.setCookingTime(time);
	}

	setDataChanged(row, RecipeRoles::CookingTimeInMinRole);
	setDataChanged(row, RecipeRoles::EverythingSetRole);
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

        newIndex = (qint32)handle.data().getRecipeIndex(newName);
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

        Recipe& rRecipe = handle.data().getRecipeAt((quint32)row);
		handle.data().renameRecipe(rRecipe, newName);
	}

	if(row != newIndex)
	{
		endMoveRows();
	}

	dataChanged(index(newIndex), index(newIndex));

	return newIndex;
}

bool ListModelRecipes::canRecipesBeAdded() const
{
	return true;
}

int ListModelRecipes::addRecipe(QString strRecipe)
{
	if(!canRecipesBeAdded())
	{
		return -1;
	}

	qint32 index = -1;
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);

		if(handle.data().existsRecipe(strRecipe))
		{
			return -1;
		}

        index = (qint32)handle.data().getRecipeIndex(strRecipe);
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

        index = (qint32)handle.data().getRecipeIndex(strRecipe);
	}

	beginInsertRows(QModelIndex(),index, index);

	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);
        const Recipe& rRecipe = handle.data().getRecipeAt((quint32)row);
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
        Recipe& rRecipe = handle.data().getRecipeAt((quint32)row);
		bSuccess = handle.data().removeRecipe(rRecipe);
	}

	endRemoveRows();

	emit recipeRemoved(row);

	return bSuccess;
}

void ListModelRecipes::onDataReset()
{
	beginResetModel();
	endResetModel();
}
