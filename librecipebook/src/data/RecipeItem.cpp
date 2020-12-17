#include "data/RecipeItem.h"
#include "data/RBElementId.h"
#include "data/AlternativesType.h"

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

bool RecipeItem::operator==(const RecipeItem& rOther) const
{
	if(getName() != rOther.getName())
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

	if(m_pAlternativesGroup == nullptr || rOther.m_pAlternativesGroup == nullptr)
	{
		return m_pAlternativesGroup == rOther.m_pAlternativesGroup;
	}
	else
	{
		return *m_pAlternativesGroup == *rOther.m_pAlternativesGroup;
	}
}
