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
