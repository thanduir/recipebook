#ifndef RECIPEBOOK_SHOPPINGLISTITEM_H
#define RECIPEBOOK_SHOPPINGLISTITEM_H

#include <QString>
#include "Amount.h"
#include "Ingredient.h"
#include "Size.h"
#include "Status.h"

namespace recipebook
{
	class RecipeItem;
	class ShoppingRecipe;
	class AlternativesType;

	class ShoppingListItem
	{
	public:
		static RBElementId getElementId(const Ingredient* pIngredient, const AlternativesType* pAlternativesGroup);

		QString getName() const { return m_pIngredient->getName(); }
		RBElementId getElementId() const { return getElementId(m_pIngredient, m_pAlternativesGroup); }

		const Ingredient& getIngredient() const { return *m_pIngredient; }
		void setIngredient(const Ingredient& rIngredient) { m_pIngredient = &rIngredient; }

		const Amount& getAmount() const { return m_Amount; }
		Amount& getAmount() { return m_Amount; }

		QString getAdditionalInfo() const { return m_AdditionalInfo; }
		void setAdditionInfo(QString additionalInfo) { m_AdditionalInfo = additionalInfo; }

		Size getSize() const { return m_Size; }
		void setSize(Size size) { m_Size = size; }

		bool isOptional() const { return m_bOptional; }
		void setIsOptional(bool optional) { m_bOptional = optional; }
		
		bool hasAlternativesGroup() const { return m_pAlternativesGroup != nullptr; }
		const AlternativesType& getAlternativesGroup() const { return *m_pAlternativesGroup; }
		void resetAlternativesGroup();
		void setAlternativesGroup(const AlternativesType& rGroup);

		Status getStatus() const { return m_Status; }
		void setStatus(Status status) { m_Status = status; }
		void invertStatus();

		bool isItemEnabled() const { return m_bItemEnabled; }
		void setItemEnabled(bool bEnabled) { m_bItemEnabled = bEnabled; }

		QString getParentRecipeName() const;

	private:
		ShoppingListItem(ShoppingRecipe& rParent, const Ingredient& rIngredient) : m_rParent(rParent), m_pIngredient(&rIngredient) {}
		ShoppingListItem(ShoppingRecipe& rParent, const RecipeItem& rRecipeItem);
		ShoppingListItem(const ShoppingListItem& rOther) = delete;

		void operator=(const ShoppingListItem& rOther) = delete;

		static RBElementId getElementId(const Ingredient* pIngredient, const RBElementId& strNewGroupIdString);
		static RBElementId getElementId(const RBElementId& strNewIngredientIdString, const AlternativesType* pAlternativesGroup);
		static RBElementId getElementId(const RBElementId& strIngredientIdString, const RBElementId& strGroupIdString);

	private:
		ShoppingRecipe& m_rParent;
		const Ingredient* m_pIngredient;
		Amount m_Amount;
		QString m_AdditionalInfo;
		Size m_Size = Size::Normal;
		bool m_bOptional = false;
		const AlternativesType* m_pAlternativesGroup = nullptr;
		Status m_Status = Status::None;
		bool m_bItemEnabled = true;

		friend class ShoppingRecipe;
	};
}

#endif
