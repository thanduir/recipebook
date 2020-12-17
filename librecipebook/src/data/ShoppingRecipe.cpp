#include "data/ShoppingRecipe.h"
#include "data/Ingredient.h"
#include "data/Recipe.h"
#include "data/RecipeItem.h"
#include "data/ShoppingListItem.h"
#include "ItemsUtil.h"

using namespace recipebook;

ShoppingRecipe::ShoppingRecipe(QString strName, const Recipe& rRecipe)
:	m_Name(strName), 
    m_fScalingFactor(static_cast<float>(rRecipe.getNumberOfPersons()))
{
	for(quint32 i = 0; i < rRecipe.getRecipeItemsCount(); ++i)
	{
		const RecipeItem& rItem = rRecipe.getRecipeItemAt(i);
		addItem(rItem);
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
	return internal::sorted::addItem<ShoppingListItem>(ShoppingListItem::getElementId(&rIngredient, nullptr), m_Items, [this, &rIngredient]()
	{
		return new ShoppingListItem(*this, rIngredient);
	});
}

ShoppingListItem& ShoppingRecipe::addItem(const RecipeItem& rItem)
{
	const AlternativesType* pType = rItem.hasAlternativesGroup() ? &rItem.getAlternativesGroup() : nullptr;
	return internal::sorted::addItem<ShoppingListItem>(ShoppingListItem::getElementId(&rItem.getIngredient(), pType), m_Items, [this, &rItem]()
	{
		return new ShoppingListItem(*this, rItem);
	});
}

bool ShoppingRecipe::existsItem(const Ingredient& rIngredient) const
{
	for(QSharedPointer<ShoppingListItem> spItem : qAsConst(m_Items))
	{
		if(helper::compare(spItem->getIngredient().getElementId(), rIngredient.getElementId()) == 0)
		{
			return true;
		}
	}

	return false;
}

bool ShoppingRecipe::removeItem(const ShoppingListItem& rItem)
{
	internal::sorted::remove(rItem.getElementId(), m_Items);
	return true;
}

ShoppingListItem& ShoppingRecipe::getItem(const Ingredient& rIngredient)
{
	for(QSharedPointer<ShoppingListItem> spItem : qAsConst(m_Items))
	{
		if(helper::compare(spItem->getIngredient().getElementId(), rIngredient.getElementId()) == 0)
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
		if(helper::compare(spItem->getIngredient().getElementId(), rIngredient.getElementId()) == 0)
		{
			return *spItem;
		}
	}

	throw QException();
}

quint32 ShoppingRecipe::getItemsCount() const
{
    return (quint32)m_Items.size();
}

ShoppingListItem& ShoppingRecipe::getItemAt(quint32 i)
{
	if(i >= (quint32)m_Items.size())
	{
		throw QException();
	}
    return *m_Items.at((int)i).get();
}

const ShoppingListItem& ShoppingRecipe::getItemAt(quint32 i) const
{
	if(i >= (quint32)m_Items.size())
	{
		throw QException();
	}
    return *m_Items.at((int)i).get();
}

quint32 ShoppingRecipe::getItemIndex(const Ingredient& rIngredient) const
{
    return (quint32)recipebook::internal::helper::findItem(ShoppingListItem::getElementId(&rIngredient, nullptr), m_Items);
}

void ShoppingRecipe::onIngredientNameChanged(const Ingredient& rIngredient)
{
	bool sortingNeeded = false;
	for(QSharedPointer<ShoppingListItem> spItem : qAsConst(m_Items))
	{
		if(helper::compare(spItem->getIngredient().getElementId(), rIngredient.getElementId()) == 0)
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

bool ShoppingRecipe::operator==(const ShoppingRecipe& rOther) const
{
	if(m_Name != rOther.m_Name)
	{
		return false;
	}

	if(m_fScalingFactor != rOther.m_fScalingFactor)
	{
		return false;
	}

	if(m_DueDate != rOther.m_DueDate)
	{
		return false;
	}

	if(m_Items.size() != rOther.m_Items.size())
	{
		return false;
	}

	for(int i = 0; i < m_Items.size(); ++i)
	{
		if(*m_Items[i] != *rOther.m_Items[i])
		{
			return false;
		}
	}

	return true;
}
