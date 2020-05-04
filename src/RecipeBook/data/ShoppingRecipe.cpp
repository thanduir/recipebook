#include "ShoppingRecipe.h"
#include "Ingredient.h"
#include "Recipe.h"
#include "RecipeItem.h"
#include "ShoppingListItem.h"
#include "util/ItemsUtil.h"

using namespace recipebook;

ShoppingRecipe::ShoppingRecipe(const Recipe& rRecipe)
:	m_Name(rRecipe.getName()), 
	m_fScalingFactor(rRecipe.getNumberOfPersons())
{
	for(quint32 i = 0; i < rRecipe.getRecipeItemsCount(); ++i)
	{
		const RecipeItem& rItem = rRecipe.getRecipeItemAt(i);
		internal::sorted::addItem(rItem.getName(), m_Items, [&rItem]()
		{
			return new ShoppingListItem(rItem);
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
	return internal::sorted::addItem(rIngredient.getName(), m_Items, [&rIngredient]()
	{
		return new ShoppingListItem(rIngredient);
	});
}

ShoppingListItem& ShoppingRecipe::addItem(const RecipeItem& rItem)
{
	return internal::sorted::addItem(rItem.getName(), m_Items, [&rItem]()
	{
		return new ShoppingListItem(rItem);
	});
}

bool ShoppingRecipe::existsItem(const Ingredient& rIngredient) const
{
	return internal::sorted::exists<ShoppingListItem>(rIngredient.getName(), m_Items);
}

bool ShoppingRecipe::removeItem(const ShoppingListItem& rItem)
{
	internal::sorted::remove(rItem, m_Items);
	return true;
}

ShoppingListItem& ShoppingRecipe::getItem(const Ingredient& rIngredient)
{
	return internal::sorted::getItem(rIngredient.getName(), m_Items);
}

const ShoppingListItem& ShoppingRecipe::getItem(const Ingredient& rIngredient) const
{
	return internal::sorted::getItem(rIngredient.getName(), m_Items);
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
	auto iter = recipebook::internal::helper::findItemSorted(rIngredient.getName(), m_Items);
	return iter - m_Items.begin();
}
