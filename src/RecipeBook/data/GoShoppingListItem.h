#ifndef RECIPEBOOK_GO_SHOPPING_LIST_ITEM_H
#define RECIPEBOOK_GO_SHOPPING_LIST_ITEM_H

#include <QVector>
#include <QDate>
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

	/* TODO:
		* nötiges haltbarkeitsdatum / wann man etwas braucht datum in shoppinglist (duedate)
		* gleiche zutaten mit untersch. eigenschaften (z.B. klein/gross, add. info.)
			trotzdem zusammenfassen, aber darunter aufzählung der einzelnen sachen (e.g. * 3 Peperoni,
			darunter eine zeile "2 gross" und eine "1 klein")
		* show somehow in "go shopping" from which shopping recipe the items are coming! (at least as a tooltip, but it'd be better if it were always visible)
			-> can be read through the parent every ShoppingListItem saves!
	*/
	// TODO(phiwid): Rückgabe von (und Abfrage auf) Unterelemente von RecipeBook aus public-Methoden braucht ein Handle!

	class GoShoppingListItem
	{
	public:
		QString getName() const { return m_Name; }
		QString getIdString() const { return m_IdString; }
		GoShoppingListItemType getType() const { return m_Type; }

		Status getStatus() const { return m_Status; }

	private:
		GoShoppingListItem(GoShoppingListItemType type, QString strName);
		explicit GoShoppingListItem(const Ingredient& rIngredient);
		GoShoppingListItem(const GoShoppingListItem& rOther) = delete;

		void operator=(const GoShoppingListItem& rOther) = delete;

		void addShoppingListItem(ShoppingListItem& rItem);
		void updateStatus(Status newStatus);

		const Ingredient& getIngredient();

	private:
		QString						m_Name;
		QString						m_IdString;
		GoShoppingListItemType		m_Type;
		const Ingredient*			m_pIngredient;

		Status						m_Status = Status::None;
		//QDate						m_DueDate;
		//Amount					m_Amount;

		QVector<ShoppingListItem*>	m_pItems;

		friend class SortedShoppingList;
	};
}

#endif
