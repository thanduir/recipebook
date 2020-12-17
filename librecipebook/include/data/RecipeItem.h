#ifndef RECIPEBOOK_RECIPE_ITEM_H
#define RECIPEBOOK_RECIPE_ITEM_H

#include <QString>
#include "Amount.h"
#include "Ingredient.h"
#include "Size.h"

namespace recipebook
{
	class AlternativesType;

	class RecipeItem
	{
	public:
		static RBElementId getElementId(const Ingredient* pIngredient);

		QString getName() const { return m_pIngredient->getName(); }
		RBElementId getElementId() const { return getElementId(m_pIngredient); }

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
		void resetAlternativesGroup() { m_pAlternativesGroup = nullptr; }
		void setAlternativesGroup(const AlternativesType& rGroup) { m_pAlternativesGroup = &rGroup; }

		bool operator!=(const RecipeItem& rOther) const { return !(*this == rOther); }
		bool operator==(const RecipeItem& rOther) const;

	private:
		explicit RecipeItem(const Ingredient& rIngredient) : m_pIngredient(&rIngredient) { m_Amount.setUnit(rIngredient.getDefaultUnit()); }
		RecipeItem(const RecipeItem& rOther);

		void operator=(const RecipeItem& rOther) = delete;

		static RBElementId getElementId(QString strName);

	private:
		const Ingredient* m_pIngredient;
		Amount m_Amount;
		QString m_AdditionalInfo;
		Size m_Size = Size::Normal;
		bool m_bOptional = false;
		const AlternativesType* m_pAlternativesGroup = nullptr;

		friend class Recipe;
	};
}

#endif
