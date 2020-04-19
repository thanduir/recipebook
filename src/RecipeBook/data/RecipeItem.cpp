#include "RecipeItem.h"

using namespace recipebook;

RecipeItem::RecipeItem(const RecipeItem& rOther)
:   m_pIngredient(&rOther.getIngredient()), 
    m_Amount(rOther.getAmount()), 
    m_AdditionalInfo(rOther.getAdditionalInfo()), 
    m_Size(rOther.getSize()), 
    m_bOptional(rOther.isOptional()),
    m_pAlternativesGroup(rOther.m_pAlternativesGroup)
{
}
