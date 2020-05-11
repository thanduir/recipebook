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
	for(QSharedPointer<ShoppingListItem> spItem : qAsConst(m_Items))
	{
		if(internal::helper::compare(spItem->getIngredient().getIdString(), rIngredient.getIdString()) == 0)
		{
			return true;
		}
	}

	return false;
}

bool ShoppingRecipe::removeItem(const ShoppingListItem& rItem)
{
	internal::sorted::remove(rItem.getIdString(), m_Items);
	return true;
}

ShoppingListItem& ShoppingRecipe::getItem(const Ingredient& rIngredient)
{
	for(QSharedPointer<ShoppingListItem> spItem : qAsConst(m_Items))
	{
		if(internal::helper::compare(spItem->getIngredient().getIdString(), rIngredient.getIdString()) == 0)
		{
			return *spItem;
		}
	}

	throw QException();
}

const ShoppingListItem& ShoppingRecipe::getItem(const Ingredient& rIngredient) const
{
	for(QSharedPointer<ShoppingListItem> spItem : qAsConst(m_Items))
	{
		if(internal::helper::compare(spItem->getIngredient().getIdString(), rIngredient.getIdString()) == 0)
		{
			return *spItem;
		}
	}

	throw QException();
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

void ShoppingRecipe::onIngredientNameChanged(const Ingredient& rIngredient)
{
	bool sortingNeeded = false;
	for(QSharedPointer<ShoppingListItem> spItem : qAsConst(m_Items))
	{
		if(internal::helper::compare(spItem->getIngredient().getIdString(), rIngredient.getIdString()) == 0)
		{
			sortingNeeded = true;
		}
	}

	if(sortingNeeded)
	{
		internal::helper::sort(m_Items);
	}
}

void ShoppingRecipe::onAlternativesTypeNameChanged(const AlternativesType& rGroup)
{
	bool sortingNeeded = false;
	for(QSharedPointer<ShoppingListItem> spItem : qAsConst(m_Items))
	{
		if(spItem->hasAlternativesGroup() && &spItem->getAlternativesGroup() == &rGroup)
		{
			sortingNeeded = true;
		}
	}

	if(sortingNeeded)
	{
		internal::helper::sort(m_Items);
	}
}

void ShoppingRecipe::onItemIdStringChanged()
{
	internal::helper::sort(m_Items);
}
