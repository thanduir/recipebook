#include "RecipeItem.h"
#include "util/RBElementId.h"

using namespace recipebook;

RBElementId RecipeItem::getElementId(const Ingredient* pIngredient)
{
	if(pIngredient != nullptr)
	{
		return pIngredient->getElementId();
	}
	return getElementId("");
}

RBElementId RecipeItem::getElementId(QString strName)
{
	return Ingredient::getElementId(strName);
}

RecipeItem::RecipeItem(const RecipeItem& rOther)
:	m_pIngredient(&rOther.getIngredient()), 
	m_Amount(rOther.getAmount()), 
	m_AdditionalInfo(rOther.getAdditionalInfo()), 
	m_Size(rOther.getSize()), 
	m_bOptional(rOther.isOptional()),
	m_pAlternativesGroup(rOther.m_pAlternativesGroup)
{
}
