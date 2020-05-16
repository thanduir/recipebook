#ifndef RECIPEBOOK_SORTED_SHOPPING_LIST_H
#define RECIPEBOOK_SORTED_SHOPPING_LIST_H

#include <QMap>
#include <QSharedPointer>
#include "SortedShoppingListOrdering.h"
#include "Status.h"

namespace recipebook
{
	class RecipeBook;
	class SortOrder;
	class Category;
	class GoShoppingListItem;
	class RBDataHandler;

	class SortedShoppingList
	{
	public:
		class ListChangeCallback
		{
		public:
			virtual void itemChanged(int posItem) = 0;

			virtual void beginMoveItem(int currentItemPos, int newItemPos) = 0;
			virtual void endMoveItem() = 0;

			virtual void beginInsertItem(int posItem) = 0;
			virtual void endInsertItem() = 0;

			virtual void beginRemoveItem(int posItem) = 0;
			virtual void endRemoveItem() = 0;
		};

	public:
		SortedShoppingList() {}

		void updateList(RecipeBook& rRecipeBook);
		void changeSortOrder(const SortOrder& rOrder, SortedShoppingListOrdering ordering);
		
		void clearList();
		
		quint32 getItemsCount() const { return m_SortedListChecked.size() + m_SortedListUnchecked.size(); }
		GoShoppingListItem& getItemAt(quint32 i);
		const GoShoppingListItem& getItemAt(quint32 i) const;

		void updateStatus(RBDataHandler& rRBDataHandler, quint32 iItem, Status status, ListChangeCallback* pCallback = nullptr);
		
	private:
		SortedShoppingList(const SortedShoppingList& rOther) = delete;
		void operator=(const SortedShoppingList& rOther) = delete;

	private:
		QMap<const Category*, QVector<QSharedPointer<GoShoppingListItem>>> m_CombinedList;
		const SortOrder*							m_pSortOrder = nullptr;
		SortedShoppingListOrdering					m_ListOrdering = SortedShoppingListOrdering::Combined;
		QVector<QSharedPointer<GoShoppingListItem>> m_SortedListChecked;
		QVector<QSharedPointer<GoShoppingListItem>> m_SortedListUnchecked;
	};
}

#endif
