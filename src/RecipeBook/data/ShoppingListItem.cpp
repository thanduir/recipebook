#include "ShoppingListItem.h"
#include "RecipeItem.h"

using namespace recipebook;

ShoppingListItem::ShoppingListItem(const RecipeItem& rRecipeItem)
:   m_pIngredient(&rRecipeItem.getIngredient()),
    m_Amount(rRecipeItem.getAmount()),
    m_AdditionalInfo(rRecipeItem.getAdditionalInfo()),
    m_Size(rRecipeItem.getSize()),
    m_bOptional(rRecipeItem.isOptional()),
    m_Status(Status::None)
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
