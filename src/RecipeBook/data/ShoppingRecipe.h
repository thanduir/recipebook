#ifndef RECIPEBOOK_SHOPPINGRECIPE_H
#define RECIPEBOOK_SHOPPINGRECIPE_H

#include <QDate>
#include <QString>
#include <QSharedPointer>
#include "util/RBElementId.h"

namespace recipebook
{
	class AlternativesType;
	class Ingredient;
	class Recipe;
	class RecipeItem;
	class ShoppingListItem;

	class ShoppingRecipe
	{
	public:
		QString getName() const { return m_Name; }
		RBElementId getElementId() const { return getElementId(getName()); }
        
		// Current scaling factor used for the items in the list.
		float getScalingFactor() const { return m_fScalingFactor; }
		// Changes the scaling factor without adjusting the amount of the items in this recipe.
		void setScalingFactor(float factor) { m_fScalingFactor = factor; }
		// Scales the amount of every item in this recipe.
		void changeScalingFactor(float f);

		QDate getDueDate() const { return m_DueDate; }
		void setDueDate(QDate dueDate) { m_DueDate = dueDate; }

		ShoppingListItem& addItem(const Ingredient& rIngredient);
		ShoppingListItem& addItem(const RecipeItem& rItem);
		bool existsItem(const Ingredient& rIngredient) const;
		bool removeItem(const ShoppingListItem& rItem);
		ShoppingListItem& getItem(const Ingredient& rIngredient);
		const ShoppingListItem& getItem(const Ingredient& rIngredient) const;

		quint32 getItemsCount() const;
		ShoppingListItem& getItemAt(quint32 i);
		const ShoppingListItem& getItemAt(quint32 i) const;
		quint32 getItemIndex(const Ingredient& rIngredient) const;

	private:
		explicit ShoppingRecipe(QString strName, float fScalingFactor) 
			: m_Name(strName), m_fScalingFactor(fScalingFactor), m_DueDate(QDate::currentDate()) {}
		explicit ShoppingRecipe(QString strName, const Recipe& rRecipe);

		ShoppingRecipe(const ShoppingRecipe& rOther) = delete;
		void operator=(const ShoppingRecipe& rOther) = delete;

		void rename(QString strNewName) { m_Name = strNewName; }

		void onIngredientNameChanged(const Ingredient& rIngredient);
		void onAlternativesTypeNameChanged(const AlternativesType& rGroup);
		void onItemIdStringChanged();

		static RBElementId getElementId(QString strName) { return RBElementId(strName); }

	private:
		QString m_Name;
		float m_fScalingFactor;
		QDate m_DueDate;

		QVector<QSharedPointer<ShoppingListItem>> m_Items;

		friend class ShoppingListItem;
		friend class RecipeBook;
	};
}

#endif
