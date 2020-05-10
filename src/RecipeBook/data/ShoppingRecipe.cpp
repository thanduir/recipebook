#include "ShoppingRecipe.h"
#include "Ingredient.h"
#include "Recipe.h"
#include "RecipeItem.h"
#include "ShoppingListItem.h"
#include "util/ItemsUtil.h"

using namespace recipebook;

ShoppingRecipe::ShoppingRecipe(QString strName, const Recipe& rRecipe)
:	m_Name(strName), 
	m_fScalingFactor(rRecipe.getNumberOfPersons())
{
	for(quint32 i = 0; i < rRecipe.getRecipeItemsCount(); ++i)
	{
		const RecipeItem& rItem = rRecipe.getRecipeItemAt(i);
		internal::sorted::addItem(rItem.getIdString(), m_Items, [this, &rItem]()
		{
			return new ShoppingListItem(*this, rItem);
		});
	}
}

void ShoppingRecipe::changeScalingFactor(float f)
{
	float fFactor = f / getScalingFactor();
	for(QSharedPointer<ShoppingListItem> spItem : qAsConst(m_Items))
	{
		Amount& amount = spItem->getAmount();
		amount.scaleAmount(fFactor);
	}

	setScalingFactor(f);
}

ShoppingListItem& ShoppingRecipe::addItem(const Ingredient& rIngredient)
{
	return internal::sorted::addItem(ShoppingListItem::getIdString(&rIngredient, nullptr), m_Items, [this, &rIngredient]()
	{
		return new ShoppingListItem(*this, rIngredient);
	});
}

ShoppingListItem& ShoppingRecipe::addItem(const RecipeItem& rItem)
{
	return internal::sorted::addItem(rItem.getIdString(), m_Items, [this, &rItem]()
	{
		return new ShoppingListItem(*this, rItem);
	});
}

bool ShoppingRecipe::existsItem(const Ingredient& rIngredient) const
{
	return internal::unsorted::exists<ShoppingListItem>(ShoppingListItem::getIdString(&rIngredient, nullptr), m_Items);
}

bool ShoppingRecipe::removeItem(const ShoppingListItem& rItem)
{
	internal::sorted::remove(rItem.getIdString(), m_Items);
	return true;
}

ShoppingListItem& ShoppingRecipe::getItem(const Ingredient& rIngredient)
{
	return internal::unsorted::getItem(ShoppingListItem::getIdString(&rIngredient, nullptr), m_Items);
}

const ShoppingListItem& ShoppingRecipe::getItem(const Ingredient& rIngredient) const
{
	return internal::unsorted::getItem(ShoppingListItem::getIdString(&rIngredient, nullptr), m_Items);
}

quint32 ShoppingRecipe::getItemsCount() const
{
	return m_Items.size();
}

ShoppingListItem& ShoppingRecipe::getItemAt(quint32 i)
{
	if(i >= (quint32)m_Items.size())
	{
		throw QException();
	}
	return *m_Items.at(i).get();
}

const ShoppingListItem& ShoppingRecipe::getItemAt(quint32 i) const
{
	if(i >= (quint32)m_Items.size())
	{
		throw QException();
	}
	return *m_Items.at(i).get();
}

quint32 ShoppingRecipe::getItemIndex(const Ingredient& rIngredient) const
{
	return recipebook::internal::helper::findItem(ShoppingListItem::getIdString(&rIngredient, nullptr), m_Items);
}

void ShoppingRecipe::beforeIngredientNameChanged(const Ingredient& rIngredient, QString strNewIdString)
{
	if(existsItem(rIngredient))
	{
		ShoppingListItem& rItem = getItem(rIngredient);
		QString idString = ShoppingListItem::getIdString(strNewIdString, 
														 rItem.hasAlternativesGroup() ? &rItem.getAlternativesGroup() : nullptr);
		internal::sorted::moveForNewIdString(rItem, idString, m_Items);
	}
}

void ShoppingRecipe::beforeAlternativesTypeNameChanged(const AlternativesType& rGroup, QString strNewIdString)
{
	QVector<QSharedPointer<ShoppingListItem>> itemsCopy = m_Items;
	for(QSharedPointer<ShoppingListItem> spItem : qAsConst(itemsCopy))
	{
		if(spItem->hasAlternativesGroup() && &spItem->getAlternativesGroup() == &rGroup)
		{
			internal::sorted::moveForNewIdString(*spItem, ShoppingListItem::getIdString(&spItem->getIngredient(), strNewIdString), m_Items);
		}
	}
}

void ShoppingRecipe::beforeItemIdStringChanged(ShoppingListItem& rItem, QString strNewIdString)
{
	internal::sorted::moveForNewIdString(rItem, strNewIdString, m_Items);
}
