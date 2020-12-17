#include "data/ShoppingListItem.h"
#include "data/RBElementId.h"
#include "data/ShoppingRecipe.h"
#include "data/RecipeItem.h"
#include "data/AlternativesType.h"

using namespace recipebook;

RBElementId ShoppingListItem::getElementId(const Ingredient* pIngredient, const AlternativesType* pAlternativesGroup)
{
	return getElementId(pIngredient != nullptr ? pIngredient->getElementId() : Ingredient::getElementId(""), 
					   pAlternativesGroup != nullptr ? pAlternativesGroup->getElementId() : AlternativesType::getElementId(""));
}

RBElementId ShoppingListItem::getElementId(const Ingredient* pIngredient, const RBElementId& newGroupIdString)
{
	return getElementId(pIngredient != nullptr ? pIngredient->getElementId() : Ingredient::getElementId(""), newGroupIdString);
}

RBElementId ShoppingListItem::getElementId(const RBElementId& newIngredientIdString, const AlternativesType* pAlternativesGroup)
{
	return getElementId(newIngredientIdString, pAlternativesGroup != nullptr ? pAlternativesGroup->getElementId() : AlternativesType::getElementId(""));
}

RBElementId ShoppingListItem::getElementId(const RBElementId& strIngredientIdString, const RBElementId& strGroupIdString)
{
	QString result;
	if(!strGroupIdString.m_Id.isEmpty())
	{
		result = strGroupIdString.m_Id + "-";
	}
	result += strIngredientIdString.m_Id;
	return RBElementId(result);
}

ShoppingListItem::ShoppingListItem(ShoppingRecipe& rParent, const RecipeItem& rRecipeItem)
:	m_rParent(rParent),
	m_pIngredient(&rRecipeItem.getIngredient()),
	m_Amount(rRecipeItem.getAmount()),
	m_AdditionalInfo(rRecipeItem.getAdditionalInfo()),
	m_Size(rRecipeItem.getSize()),
	m_bOptional(rRecipeItem.isOptional()),
	m_pAlternativesGroup(rRecipeItem.hasAlternativesGroup() ? &rRecipeItem.getAlternativesGroup() : nullptr),
	m_Status(Status::None),
	m_bItemEnabled(!rRecipeItem.hasAlternativesGroup())
{
}

void ShoppingListItem::invertStatus()
{
	if(m_Status == Status::None)
	{
		setStatus(Status::Taken);
	}
	else
	{
		setStatus(Status::None);
	}
}

void ShoppingListItem::resetAlternativesGroup()
{
	if(m_pAlternativesGroup != nullptr)
	{
		m_pAlternativesGroup = nullptr;
		m_rParent.onItemIdStringChanged();
	}
}

void ShoppingListItem::setAlternativesGroup(const AlternativesType& rGroup)
{
	if(m_pAlternativesGroup != &rGroup)
	{
		m_pAlternativesGroup = &rGroup;
		m_rParent.onItemIdStringChanged();
	}
}

QString ShoppingListItem::getParentRecipeName() const
{
	return m_rParent.getName();
}

bool ShoppingListItem::operator==(const ShoppingListItem& rOther) const
{
	if(getName() != rOther.getName())
	{
		return false;
	}

	if(m_rParent.getName() != rOther.m_rParent.getName())
	{
		return false;
	}
	
	if(m_Amount != rOther.m_Amount)
	{
		return false;
	}

	if(m_AdditionalInfo != rOther.m_AdditionalInfo)
	{
		return false;
	}

	if(m_Size != rOther.m_Size)
	{
		return false;
	}

	if(m_bOptional != rOther.m_bOptional)
	{
		return false;
	}

	if(m_Status != rOther.m_Status)
	{
		return false;
	}

	if(m_bItemEnabled != rOther.m_bItemEnabled)
	{
		return false;
	}

	if(m_pAlternativesGroup == nullptr || rOther.m_pAlternativesGroup == nullptr)
	{
		return m_pAlternativesGroup == rOther.m_pAlternativesGroup;
	}
	else
	{
		return *m_pAlternativesGroup == *rOther.m_pAlternativesGroup;
	}
}
