#ifndef RECIPEBOOK_GO_SHOPPING_LIST_ITEM_H
#define RECIPEBOOK_GO_SHOPPING_LIST_ITEM_H

#include <QVector>
#include <QDate>
#include "Amount.h"
#include "Size.h"
#include "Status.h"

namespace recipebook
{
	class Ingredient;
	class ShoppingListItem;

	enum class GoShoppingListItemType
	{
		Toplevel_Header_CheckedItems,
		Toplevel_Header_UncheckedItems,
		Category_Header,
		Incompatible_Items_Header,
		IngredientListItem
	};

	class GoShoppingListItem
	{
	public:
		struct RecipeInfo
		{
			QString m_RecipeName;
			Amount	m_Amount;
			QDate	m_DueDate;
		};

	public:
		QString getName() const { return m_Name; }
		QString getIdString() const { return m_IdString; }
		GoShoppingListItemType getType() const { return m_Type; }

		Status getStatus() const { return m_Status; }
		const QVector<Amount>& getAmount() const { return m_Amount; }
		bool isOptional() const { return m_bOptional; }

		quint32 getCombinedItemsCount() const { return m_CombinedItems.size(); }
		Amount getAmount(quint32 i) const;
		bool isOptional(quint32 i) const;
		Size getSize(quint32 i) const;
		QString getAdditionalInfo(quint32 i) const;

		const QVector<RecipeInfo>& getRecipeInfos() const { return m_RecipeInfo; }

	private:
		GoShoppingListItem(GoShoppingListItemType type, QString strName);
		explicit GoShoppingListItem(const Ingredient& rIngredient);
		GoShoppingListItem(const GoShoppingListItem& rOther) = delete;

		void operator=(const GoShoppingListItem& rOther) = delete;

		void addShoppingListItem(ShoppingListItem& rItem, QDate dueDate);
		void updateStatus(Status newStatus);
		const Ingredient& getIngredient();

	private:
		struct CombinedItem
		{
			Amount						m_Amount;
			bool						m_bOptional;
			Size						m_Size;
			QString						m_AdditionalInfo;

			QVector<ShoppingListItem*>	m_pItems;
		};

		bool itemCompatible(const ShoppingListItem& rItem, const CombinedItem& rCombined)  const;

	private:
		QString						m_Name;
		QString						m_IdString;
		GoShoppingListItemType		m_Type;
		const Ingredient*			m_pIngredient;

		Status						m_Status = Status::None;
		QVector<Amount>				m_Amount;
		bool						m_bOptional = false;

		QVector<CombinedItem>		m_CombinedItems;

		QVector<RecipeInfo>			m_RecipeInfo;

		friend class SortedShoppingList;
	};
}

#endif
