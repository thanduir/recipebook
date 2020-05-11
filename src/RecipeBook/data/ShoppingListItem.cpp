#include "ShoppingListItem.h"
#include "ShoppingRecipe.h"
#include "RecipeItem.h"
#include "AlternativesType.h"

using namespace recipebook;

QString ShoppingListItem::getIdString(const Ingredient* pIngredient, const AlternativesType* pAlternativesGroup)
{
	return getIdString(pIngredient != nullptr ? pIngredient->getIdString() : "", pAlternativesGroup != nullptr ? pAlternativesGroup->getIdString() : "");
}

QString ShoppingListItem::getIdString(const Ingredient* pIngredient, QString strNewGroupIdString)
{
	return getIdString(pIngredient != nullptr ? pIngredient->getIdString() : "", strNewGroupIdString);
}

QString ShoppingListItem::getIdString(QString strNewIngredientIdString, const AlternativesType* pAlternativesGroup)
{
	return getIdString(strNewIngredientIdString, pAlternativesGroup != nullptr ? pAlternativesGroup->getIdString() : "");
}

QString ShoppingListItem::getIdString(QString strIngredientIdString, QString strGroupIdString)
{
	QString result;
	if(strGroupIdString != "")
	{
		result = strGroupIdString + "-";
	}
	if(strIngredientIdString != "")
	{
		result += strIngredientIdString;
	}
	return result;
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
