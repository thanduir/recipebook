#include "data/GoShoppingListItem.h"
#include <QException>
#include "data/Ingredient.h"
#include "data/ShoppingListItem.h"

using namespace recipebook;

GoShoppingListItem::GoShoppingListItem(GoShoppingListItemType type, QString strName)
:	m_Name(strName),
	m_ElementId(strName),
	m_Type(type),
	m_pIngredient(nullptr)
{
}

GoShoppingListItem::GoShoppingListItem(const Ingredient& rIngredient)
:	m_Name(rIngredient.getName()),
	m_ElementId(rIngredient.getElementId()),
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

bool GoShoppingListItem::itemCompatible(const ShoppingListItem& rItem, const CombinedItem& rCombined)  const
{
	if(rItem.isOptional() != rCombined.m_bOptional
	   || rItem.getSize() != rCombined.m_Size
	   || rItem.getAdditionalInfo() != rCombined.m_AdditionalInfo)
	{
		return false;
	}

	return Amount::canBeAddedUp(rItem.getAmount(), rCombined.m_Amount);
}

void GoShoppingListItem::addShoppingListItem(ShoppingListItem& rItem, QDate dueDate)
{
	// Status
	if(m_CombinedItems.size() == 1)
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

	// Amount
	bool bAdded = false;
	for(Amount& rAmount : m_Amount)
	{
		if(Amount::canBeAddedUp(rAmount, rItem.getAmount()))
		{
			bAdded = true;
			rAmount.add(rItem.getAmount());
		}
	}
	if(!bAdded)
	{
		m_Amount.append(rItem.getAmount());
	}

	// Optional
	if(m_CombinedItems.size() > 0)
	{
		m_bOptional = m_bOptional && rItem.isOptional();
	}
	else
	{
		m_bOptional = rItem.isOptional();
	}

	// Recipe infos
	RecipeInfo info;
	info.m_RecipeName = rItem.getParentRecipeName();
	info.m_Amount = rItem.getAmount();
	info.m_DueDate = dueDate;
	m_RecipeInfo.append(info);

	// Combined items

	for(CombinedItem& rCombinedItem : m_CombinedItems)
	{
		if(itemCompatible(rItem, rCombinedItem))
		{
			rCombinedItem.m_Amount.add(rItem.getAmount());
			rCombinedItem.m_pItems.append(&rItem);
			return;
		}
	}

	CombinedItem newItem;
	newItem.m_Amount = rItem.getAmount();
	newItem.m_bOptional = rItem.isOptional();
	newItem.m_Size = rItem.getSize();
	newItem.m_AdditionalInfo = rItem.getAdditionalInfo();
	newItem.m_pItems.append(&rItem);
	m_CombinedItems.append(newItem);
}

void GoShoppingListItem::updateStatus(Status newStatus)
{
	m_Status = newStatus;

	for(const CombinedItem& rCombinedItem : qAsConst(m_CombinedItems))
	{
		for(ShoppingListItem* pItem : qAsConst(rCombinedItem.m_pItems))
		{
			pItem->setStatus(newStatus);
		}
	}
}

Amount GoShoppingListItem::getAmount(quint32 i) const
{
	if(i >= (quint32)m_CombinedItems.size())
	{
		throw QException();
	}

	return m_CombinedItems[i].m_Amount;
}

bool GoShoppingListItem::isOptional(quint32 i) const
{
	if(i >= (quint32)m_CombinedItems.size())
	{
		throw QException();
	}

	return m_CombinedItems[i].m_bOptional;
}

Size GoShoppingListItem::getSize(quint32 i) const
{
	if(i >= (quint32)m_CombinedItems.size())
	{
		throw QException();
	}

	return m_CombinedItems[i].m_Size;
}

QString GoShoppingListItem::getAdditionalInfo(quint32 i) const
{
	if(i >= (quint32)m_CombinedItems.size())
	{
		throw QException();
	}

	return m_CombinedItems[i].m_AdditionalInfo;
}
