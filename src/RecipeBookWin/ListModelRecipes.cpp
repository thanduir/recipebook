#include "ListModelRecipes.h"
#include <QException>
#include <data/RecipeBook.h>
#include <data/Recipe.h>
#include "RecipeBookSettings.h"

using namespace recipebook::UI;

ListModelRecipes::ListModelRecipes(recipebook::RecipeBook& rRecipeBook, const recipebook::RecipeBookSettings& rSettings)
:	m_rRecipeBook(rRecipeBook),
    m_rSettings(rSettings)
{
}

int ListModelRecipes::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_rRecipeBook.getRecipesCount();
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
    if(row < 0 || row >= (int) m_rRecipeBook.getRecipesCount())
        return "";

    const Recipe& rRecipe = m_rRecipeBook.getRecipeAt(row);
    return rRecipe.getName();
}

bool ListModelRecipes::isEverythingSet(int row) const
{
    if(row < 0 || row >= (int) m_rRecipeBook.getRecipesCount())
        return false;

    const Recipe& rRecipe = m_rRecipeBook.getRecipeAt(row);
    
    return rRecipe.getRecipeText() != "" 
           && rRecipe.getShortDescription() != "" 
           && rRecipe.getNumberOfPersons() > 0 
           && !rRecipe.getCookingTime().isNull()
           && rRecipe.getRecipeItemsCount() > 0;
}

quint32 ListModelRecipes::numberOfPersons(int row) const
{
    if(row < 0 || row >= (int) m_rRecipeBook.getRecipesCount())
        return 0;

    const Recipe& rRecipe = m_rRecipeBook.getRecipeAt(row);
    return rRecipe.getNumberOfPersons();
}

QString ListModelRecipes::shortDescription(int row) const
{
    if(row < 0 || row >= (int) m_rRecipeBook.getRecipesCount())
        return 0;

    const Recipe& rRecipe = m_rRecipeBook.getRecipeAt(row);
    return rRecipe.getShortDescription();
}

QString ListModelRecipes::recipeText(int row) const
{
    if(row < 0 || row >= (int) m_rRecipeBook.getRecipesCount())
        return 0;

    const Recipe& rRecipe = m_rRecipeBook.getRecipeAt(row);
    return rRecipe.getRecipeText();
}

quint32 ListModelRecipes::cookingTime(int row) const
{
    if(row < 0 || row >= (int) m_rRecipeBook.getRecipesCount())
        return 0;

    const Recipe& rRecipe = m_rRecipeBook.getRecipeAt(row);
    QTime time = rRecipe.getCookingTime();
    return time.minute() + 60 * time.hour();
}

void ListModelRecipes::setNumberOfPersons(int row, qint32 nrPersons)
{
    if (row < 0 || row >= (int)m_rRecipeBook.getRecipesCount())
        return;

    Recipe& rRecipe = m_rRecipeBook.getRecipeAt(row);
    rRecipe.setNumberOfPersons(nrPersons);

    setDataChanged(row, RecipeRoles::NumberOfPersons);
    setDataChanged(row, RecipeRoles::EverythingSet);
}

void ListModelRecipes::setShortDescription(int row, QString desc)
{
    if (row < 0 || row >= (int)m_rRecipeBook.getRecipesCount())
        return;

    Recipe& rRecipe = m_rRecipeBook.getRecipeAt(row);
    rRecipe.setShortDescription(desc);

    setDataChanged(row, RecipeRoles::ShortDescription);
    setDataChanged(row, RecipeRoles::EverythingSet);
}

void ListModelRecipes::setRecipeText(int row, QString text)
{
    if (row < 0 || row >= (int)m_rRecipeBook.getRecipesCount())
        return;

    Recipe& rRecipe = m_rRecipeBook.getRecipeAt(row);
    rRecipe.setRecipeText(text);

    setDataChanged(row, RecipeRoles::RecipeText);
    setDataChanged(row, RecipeRoles::EverythingSet);
}

void ListModelRecipes::setCookingTime(int row, quint32 timeInMin)
{
    if (row < 0 || row >= (int)m_rRecipeBook.getRecipesCount())
        return;

    Recipe& rRecipe = m_rRecipeBook.getRecipeAt(row);
    QTime time(timeInMin / 60, timeInMin % 60);
    rRecipe.setCookingTime(time);

    setDataChanged(row, RecipeRoles::CookingTimeInMin);
    setDataChanged(row, RecipeRoles::EverythingSet);
}

int ListModelRecipes::renameRecipe(int row, QString newName)
{
    if(row < 0 || row >= (int) m_rRecipeBook.getRecipesCount())
        return -1;

    if(m_rRecipeBook.existsRecipe(newName))
    {
        return -1;
    }

    qint32 newIndex = m_rRecipeBook.getRecipeIndex(newName);
    if(row != newIndex)
    {
        beginMoveRows(QModelIndex(), row, row, QModelIndex(), newIndex);
    }

    if(newIndex > row)
    {
        newIndex -= 1;
    }

    Recipe& rRecipe = m_rRecipeBook.getRecipeAt(row);
    m_rRecipeBook.renameRecipe(rRecipe, newName);

    if(row != newIndex)
    {
        endMoveRows();
    }

    dataChanged(index(newIndex), index(newIndex));

    return newIndex;
}

int ListModelRecipes::addRecipe(QString strRecipe)
{
    if(m_rRecipeBook.existsRecipe(strRecipe))
    {
        return -1;
    }

    qint32 index = m_rRecipeBook.getRecipeIndex(strRecipe);

    beginInsertRows(QModelIndex(),index, index);

    m_rRecipeBook.addRecipe(strRecipe, m_rSettings.getDefaultRecipeNrPersons());

    endInsertRows();

    return index;
}

int ListModelRecipes::copyRecipe(int row, QString strRecipe)
{
    if(row < 0 || row >= (int) m_rRecipeBook.getRecipesCount())
        return -1;

    if(m_rRecipeBook.existsRecipe(strRecipe))
    {
        return -1;
    }

    qint32 index = m_rRecipeBook.getRecipeIndex(strRecipe);

    beginInsertRows(QModelIndex(),index, index);

    const Recipe& rRecipe = m_rRecipeBook.getRecipeAt(row);
    m_rRecipeBook.copyRecipe(rRecipe, strRecipe);

    endInsertRows();

    return index;
}

bool ListModelRecipes::existsRecipe(QString strRecipe) const
{
    return m_rRecipeBook.existsRecipe(strRecipe);
}

bool ListModelRecipes::removeRecipe(int row)
{
    if(row < 0 || row >= (int) m_rRecipeBook.getRecipesCount())
    {
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row);

    Recipe& rRecipe = m_rRecipeBook.getRecipeAt(row);
    bool bSuccess = m_rRecipeBook.removeRecipe(rRecipe);

    endRemoveRows();

    return bSuccess;
}

void ListModelRecipes::onDataReset()
{
    beginResetModel();
    endResetModel();
}
