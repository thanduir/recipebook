#include "GoShoppingListItem.h"
#include <QException>
#include "data/Ingredient.h"
#include "data/ShoppingListItem.h"

using namespace recipebook;

GoShoppingListItem::GoShoppingListItem(GoShoppingListItemType type, QString strName)
:	m_Name(strName),
	m_IdString(strName),
	m_Type(type),
	m_pIngredient(nullptr)
{

}

GoShoppingListItem::GoShoppingListItem(const Ingredient& rIngredient)
:	m_Name(rIngredient.getName()),
	m_IdString(rIngredient.getIdString()),
	m_Type(GoShoppingListItemType::IngredientListItem),
	m_pIngredient(&rIngredient)
{
}

const Ingredient& GoShoppingListItem::getIngredient()
{
	if(m_pIngredient == nullptr)
	{
		throw QException();
	}

	return *m_pIngredient;
}

void GoShoppingListItem::addShoppingListItem(ShoppingListItem& rItem)
{
	m_pItems.append(&rItem);

	if(m_pItems.size() == 1)
	{
		m_Status = rItem.getStatus();
	}
	else
	{
		if(rItem.getStatus() == Status::None)
		{
			m_Status = Status::None;
		}
	}

	/*
	CategoryShoppingItem csi
	Optional<ShoppingListItem.Status> otherStatus = csi.getStatus();
	if(!otherStatus.isPresent())
	{
		continue;
	}
	if(item.isOptional() != csi.isOptional()
			|| item.getSize() != csi.getSize()
			|| item.getStatus() != otherStatus.get()
			|| !item.getAdditionalInfo().equals(csi.getAdditionalInfo()))
	{
		continue;
	}

	if(Amount.canBeAddedUp(item.getAmount(), csi.getAmount()))
	{
		Optional<Amount> addedAmounts = Amount.addUp(item.getAmount(), csi.getAmount());
		if(!addedAmounts.isPresent())
		{
			continue;
		}
		csi.setAmount(addedAmounts.get());
		csi.addShoppingListItem(item);
		return;
	}
	*/
}

void GoShoppingListItem::updateStatus(Status newStatus)
{
	m_Status = newStatus;

	for(ShoppingListItem* pItem : qAsConst(m_pItems))
	{
		pItem->setStatus(newStatus);
	}
}
