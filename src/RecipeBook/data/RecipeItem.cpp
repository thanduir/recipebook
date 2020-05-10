#include "RecipeItem.h"

using namespace recipebook;

QString RecipeItem::getIdString(const Ingredient* pIngredient)
{
	if(pIngredient != nullptr)
	{
		return pIngredient->getIdString();
	}
	return "";
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
