#include "ListModelIngredients.h"
#include <QException>
#include <data/RecipeBook.h>
#include <data/Category.h>
#include <data/Ingredient.h>
#include <data/SortOrder.h>
#include <data/Recipe.h>
#include <data/ShoppingRecipe.h>
#include "uistringconverter.h"

using namespace recipebook::UI;

ListModelIngredients::ListModelIngredients(recipebook::RecipeBook& rRecipeBook, const UIStringConverter& rConverter)
:	m_rRecipeBook(rRecipeBook),
    m_rConverter(rConverter)
{
}

int ListModelIngredients::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_rRecipeBook.getIngredientsCount();
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
    else if(role == IngredientRoles::ProvenanceRole)
    {
        return provenance(index.row());
    }
    else if(role == IngredientRoles::DefaultUnitRole)
    {
        return defaultUnit(index.row());
    }

    return QVariant();
}

QString ListModelIngredients::name(int row) const
{
    if(row < 0 || row >= (int) m_rRecipeBook.getIngredientsCount())
        return "";

    const Ingredient& rIngredient = m_rRecipeBook.getIngredientAt(row);
    return rIngredient.getName();
}

QString ListModelIngredients::category(int row) const
{
    if(row < 0 || row >= (int) m_rRecipeBook.getIngredientsCount())
        return "";

    const Ingredient& rIngredient = m_rRecipeBook.getIngredientAt(row);
    return rIngredient.getCategory().getName();
}

QString ListModelIngredients::provenance(int row) const
{
    if(row < 0 || row >= (int) m_rRecipeBook.getIngredientsCount())
        return "";

    const Ingredient& rIngredient = m_rRecipeBook.getIngredientAt(row);
    if(rIngredient.hasProvenanceEverywhere())
    {
        return m_rConverter.getProvenanceEverywhere();
    }
    else
    {
        return rIngredient.getProvenance().getName();
    }
}

QString ListModelIngredients::defaultUnit(int row) const
{
    if(row < 0 || row >= (int) m_rRecipeBook.getIngredientsCount())
        return "";

    const Ingredient& rIngredient = m_rRecipeBook.getIngredientAt(row);
    return  m_rConverter.convertUnit(rIngredient.getDefaultUnit());
}

QString ListModelIngredients::listUsedInRecipes(int row) const
{
    if(row < 0 || row >= (int) m_rRecipeBook.getIngredientsCount())
        return "";

    Ingredient& rIngredient = m_rRecipeBook.getIngredientAt(row);

    QList<Recipe*> recipes;
    QList<ShoppingRecipe*> shoppingRecipes;
    if(!m_rRecipeBook.isIngredientInUse(rIngredient, &recipes, &shoppingRecipes))
    {
        return "";
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

    return text;
}

QString ListModelIngredients::listUsedInShoppingRecipes(int row) const
{
    if(row < 0 || row >= (int) m_rRecipeBook.getIngredientsCount())
        return "";

    Ingredient& rIngredient = m_rRecipeBook.getIngredientAt(row);

    QList<Recipe*> recipes;
    QList<ShoppingRecipe*> shoppingRecipes;
    if(!m_rRecipeBook.isIngredientInUse(rIngredient, &recipes, &shoppingRecipes))
    {
        return "";
    }

    QString text;
    if(recipes.size() > 0)
    {
        text = "<ul>";
        for(ShoppingRecipe* pRecipe : qAsConst(shoppingRecipes))
        {
            text += "<li>" + pRecipe->getName() + "</li>";
        }
        text += "</ul>";
    }

    return text;
}

void ListModelIngredients::setCategory(int row, QString newCategory)
{
    if(row < 0 || row >= (int) m_rRecipeBook.getIngredientsCount())
        return;

    if(!m_rRecipeBook.existsCategory(newCategory))
        return;

    const Category& rCategory = m_rRecipeBook.getCategory(newCategory);
    Ingredient& rIngredient = m_rRecipeBook.getIngredientAt(row);
    rIngredient.setCategory(rCategory);

    setDataChanged(row, IngredientRoles::CategoryRole);
}

void ListModelIngredients::setProvenance(int row, QString newProvenance)
{
    if(row < 0 || row >= (int) m_rRecipeBook.getIngredientsCount())
        return;

    Ingredient& rIngredient = m_rRecipeBook.getIngredientAt(row);
    if(newProvenance == m_rConverter.getProvenanceEverywhere())
    {
        rIngredient.setProvenanceEverywhere();
    }
    else if(m_rRecipeBook.existsSortOrder(newProvenance))
    {
        const SortOrder& rSortOrder = m_rRecipeBook.getSortOrder(newProvenance);
        rIngredient.setProvenance(rSortOrder);
    }
    else
    {
        return;
    }

    setDataChanged(row, IngredientRoles::ProvenanceRole);
}

void ListModelIngredients::setDefaultUnit(int row, QString newDefaultUnit)
{
    if (row < 0 || row >= (int)m_rRecipeBook.getIngredientsCount())
        return;

    Unit unit = m_rConverter.convertUnit(newDefaultUnit);
    Ingredient& rIngredient = m_rRecipeBook.getIngredientAt(row);
    rIngredient.setDefaultUnit(unit);

    setDataChanged(row, IngredientRoles::DefaultUnitRole);
}

QHash<int, QByteArray> ListModelIngredients::roleNames() const
{
	QHash<int, QByteArray> roles;
    roles[(int)IngredientRoles::NameRole] = "name";
    roles[(int)IngredientRoles::CategoryRole] = "category";
	roles[(int)IngredientRoles::ProvenanceRole] = "provenance";
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
    if(row < 0 || row >= (int) m_rRecipeBook.getIngredientsCount())
        return -1;

    if(m_rRecipeBook.existsIngredient(newName))
    {
        return -1;
    }

    qint32 newIndex = m_rRecipeBook.getIngredientIndex(newName);
    if(row != newIndex)
    {
        beginMoveRows(QModelIndex(), row, row, QModelIndex(), newIndex);
    }

    if(newIndex > row)
    {
        newIndex -= 1;
    }

    Ingredient& rIngredient = m_rRecipeBook.getIngredientAt(row);
    m_rRecipeBook.renameIngredient(rIngredient, newName);

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
    if(m_rRecipeBook.existsIngredient(strIngredient))
    {
        return -1;
    }

    if(m_rRecipeBook.getCategoriesCount() == 0)
    {
        return -1;
    }

    qint32 index = m_rRecipeBook.getIngredientIndex(strIngredient);

    beginInsertRows(QModelIndex(),index, index);

    // TODO: Define sensible defaults!
    const Category& rCategory = m_rRecipeBook.getCategoryAt(0);
    Unit unit = Unit::Count;
    m_rRecipeBook.addIngredient(strIngredient, rCategory, unit);

    endInsertRows();

    return index;
}

bool ListModelIngredients::existsIngredient(QString strIngredient) const
{
    return m_rRecipeBook.existsIngredient(strIngredient);
}

bool ListModelIngredients::canIngredientBeRemoved(int row) const
{
    if(row < 0 || row >= (int) m_rRecipeBook.getIngredientsCount())
    {
        return false;
    }

    Ingredient& rIngredient = m_rRecipeBook.getIngredientAt(row);
    return !m_rRecipeBook.isIngredientInUse(rIngredient);
}

bool ListModelIngredients::removeIngredient(int row)
{
    if(row < 0 || row >= (int) m_rRecipeBook.getIngredientsCount())
    {
        return false;
    }

    if(!canIngredientBeRemoved(row))
    {
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row);

    Ingredient& rIngredient = m_rRecipeBook.getIngredientAt(row);
    bool bSuccess = m_rRecipeBook.removeIngredient(rIngredient);

    endRemoveRows();

    return bSuccess;
}

void ListModelIngredients::onCategoryRenamed(quint32 row)
{
    dataChanged(index(0), index(m_rRecipeBook.getIngredientsCount()-1));
}

void ListModelIngredients::onSortOrderRenamed(quint32 row)
{
    dataChanged(index(0), index(m_rRecipeBook.getIngredientsCount()-1));
}
