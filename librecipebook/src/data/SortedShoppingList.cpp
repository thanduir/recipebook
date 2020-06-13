#include "data/SortedShoppingList.h"
#include <QVector>
#include "data/Category.h"
#include "data/GoShoppingListItem.h"
#include "data/Ingredient.h"
#include "data/RecipeBook.h"
#include "data/ShoppingListItem.h"
#include "data/ShoppingRecipe.h"
#include "data/SortOrder.h"
#include "data/RBDataHandler.h"
#include "ItemsUtil.h"

using namespace recipebook;

namespace
{
	int getItemIndex(QString strName, GoShoppingListItemType type, const QVector<QSharedPointer<GoShoppingListItem>>& rList)
	{
		for(int i = 0; i < rList.size(); ++i)
		{
			const QSharedPointer<GoShoppingListItem> spElement = rList.at(i);
			if(spElement->getName() == strName && spElement->getType() == type)
			{
				return i;
			}
		}
		return rList.size();
	};

	int getNextItemOfType(const QVector<QSharedPointer<GoShoppingListItem>>& rList, GoShoppingListItemType type, int startPos)
	{
		for(int i = startPos; i < rList.size(); ++i)
		{
			if(rList.at(i)->getType() == type)
			{
				return i;
			}
		}
		return rList.size();
	};
}

void SortedShoppingList::updateList(RecipeBook& rRecipeBook)
{
	m_CombinedList.clear();

	for(quint32 i = 0; i < rRecipeBook.getShoppingRecipesCount(); ++i)
	{
		ShoppingRecipe& rRecipe = rRecipeBook.getShoppingRecipeAt(i);
		
		for(quint32 j = 0; j < rRecipe.getItemsCount(); ++j)
		{
			ShoppingListItem& rItem = rRecipe.getItemAt(j);
			if(!rItem.isItemEnabled())
			{
				continue;
			}

			const Ingredient& rIngredient = rItem.getIngredient();
			const Category* pCategory = &rIngredient.getCategory();

			GoShoppingListItem* pCombinedItem = nullptr;
			for(auto listItem : qAsConst(m_CombinedList[pCategory]))
			{
				if(helper::compare(listItem->getElementId(), rIngredient.getElementId()) == 0)
				{
					pCombinedItem = listItem.get();
				}
			}

			if(pCombinedItem == nullptr)
			{
				pCombinedItem = &internal::sorted::addItem<GoShoppingListItem>(rIngredient.getElementId(), m_CombinedList[pCategory], [&rIngredient]()
				{
					return new GoShoppingListItem(rIngredient);
				});
			}

			pCombinedItem->addShoppingListItem(rItem, rRecipe.getDueDate());
		}
	}
}

void SortedShoppingList::changeSortOrder(const SortOrder& rOrder, SortedShoppingListOrdering ordering)
{
	m_pSortOrder = &rOrder;
	m_ListOrdering = ordering;

	QVector<QSharedPointer<GoShoppingListItem>> checkedItems;
	QVector<QSharedPointer<GoShoppingListItem>> checkedIncompatibleItems;
	QVector<QSharedPointer<GoShoppingListItem>> uncheckedItems;
	QVector<QSharedPointer<GoShoppingListItem>> uncheckedIncompatibleItems;
	
	if(m_ListOrdering == SortedShoppingListOrdering::SeparateTakenItems)
    {
		QSharedPointer<GoShoppingListItem> hdrChecked(new GoShoppingListItem(GoShoppingListItemType::Toplevel_Header_CheckedItems, ""));
		checkedItems.append(hdrChecked);

		QSharedPointer<GoShoppingListItem> hdrUnchecked(new GoShoppingListItem(GoShoppingListItemType::Toplevel_Header_UncheckedItems, ""));
		uncheckedItems.append(hdrUnchecked);
    }

	for(quint32 i = 0; i < rOrder.getItemsCount(); ++i)
	{
		const Category* pCategory = &rOrder.getItemAt(i);

        if(m_CombinedList.contains(pCategory))
        {
            bool addedCheckedHeader = false;
            bool addedCheckedIncompatibleHeader = false;
            bool addedUncheckedHeader = false;
            bool addedUncheckedIncompatibleHeader = false;

			for(QSharedPointer<GoShoppingListItem> spCurrent : qAsConst(m_CombinedList[pCategory]))
            {
				const Ingredient& rIngredient = spCurrent->getIngredient();

				bool bCompatible = rIngredient.provenanceAvailable(rOrder);
				bool bTaken = spCurrent->getStatus() == Status::Taken;

				auto fnAddToList = [pCategory, spCurrent](bool& bHeaderAdded, QVector<QSharedPointer<GoShoppingListItem>>& rList, GoShoppingListItemType headerType)
				{
					if(!bHeaderAdded)
                    {
                        bHeaderAdded = true;
						QSharedPointer<GoShoppingListItem> header(new GoShoppingListItem(headerType, pCategory->getName()));
                        rList.append(header);
                    }
					rList.append(spCurrent);
				};

				if(bTaken || m_ListOrdering == SortedShoppingListOrdering::Combined)
                {
                    // checked
                    if(bCompatible)
                    {
						fnAddToList(addedCheckedHeader, checkedItems, GoShoppingListItemType::Category_Header);
                    }
                    else
                    {
						fnAddToList(addedCheckedIncompatibleHeader, checkedIncompatibleItems, GoShoppingListItemType::Incompatible_Items_Header);
                    }
                }
                else
                {
                    // unchecked
                    if(bCompatible)
                    {
						fnAddToList(addedUncheckedHeader, uncheckedItems, GoShoppingListItemType::Category_Header);
                    }
                    else
                    {
						fnAddToList(addedUncheckedIncompatibleHeader, uncheckedIncompatibleItems, GoShoppingListItemType::Incompatible_Items_Header);
                    }
                }
            }
        }
    }

	m_SortedListChecked.clear();
    m_SortedListChecked.append(checkedItems);
    m_SortedListChecked.append(checkedIncompatibleItems);

	m_SortedListUnchecked.clear();
    m_SortedListUnchecked.append(uncheckedItems);
    m_SortedListUnchecked.append(uncheckedIncompatibleItems);
}

void SortedShoppingList::clearList()
{
	m_SortedListChecked.clear();
	m_SortedListUnchecked.clear();
	m_CombinedList.clear();
}

GoShoppingListItem& SortedShoppingList::getItemAt(quint32 i)
{
	quint32 sizeChecked = m_SortedListChecked.size();
	if(i < sizeChecked)
	{
		return *m_SortedListChecked[i];
	}
	else if(i < ((quint32)m_SortedListUnchecked.size() + sizeChecked))
	{
		return *m_SortedListUnchecked[i - sizeChecked];
	}

	throw QException();
}

const GoShoppingListItem& SortedShoppingList::getItemAt(quint32 i) const
{
	quint32 sizeChecked = m_SortedListChecked.size();
	if(i < sizeChecked)
	{
		return *m_SortedListChecked[i];
	}
	else if(i < ((quint32)m_SortedListUnchecked.size() + sizeChecked))
	{
		return *m_SortedListUnchecked[i - sizeChecked];
	}

	throw QException();
}

void SortedShoppingList::updateStatus(RBDataHandler& rRBDataHandler, quint32 iItem, Status status, ListChangeCallback* pCallback)
{
	quint32 currentItemPos = iItem;
	GoShoppingListItem& rItem = getItemAt(currentItemPos);
	
	if(rItem.getType() != GoShoppingListItemType::IngredientListItem || rItem.getStatus() == status)
	{ 
		return;
	}

	if(m_ListOrdering == SortedShoppingListOrdering::Combined)
	{
		{
			recipebook::RBDataWriteHandle handle(rRBDataHandler);
			rItem.updateStatus(status);
		}
		if(pCallback)
		{
			pCallback->itemChanged(iItem);
		}
		return;
	}
	
	bool bCompatible = false;
	QString strCategory;
	{
		recipebook::RBDataReadHandle handle(rRBDataHandler);
		const Ingredient& rIngredient = rItem.getIngredient();
		bCompatible = rIngredient.provenanceAvailable(*m_pSortOrder);
		strCategory = rIngredient.getCategory().getName();
	}

	// Remove header directly before the element?
    if(getItemAt(currentItemPos-1).getType() != GoShoppingListItemType::IngredientListItem)
    {
		if(currentItemPos + 1 == getItemsCount() || getItemAt(currentItemPos + 1).getType() != GoShoppingListItemType::IngredientListItem)
		{
			if(pCallback)
			{
				pCallback->beginRemoveItem(currentItemPos - 1);
			}

			int posToRemove = currentItemPos - 1;
			if(posToRemove < m_SortedListChecked.size())
			{
				m_SortedListChecked.removeAt(posToRemove);
			}
			else
			{
				posToRemove -= m_SortedListChecked.size();
				m_SortedListUnchecked.removeAt(posToRemove);
			}
			--currentItemPos;

			if(pCallback)
			{
				pCallback->endRemoveItem();
			}
		}
    }

	// Find header position in new list (or add header if it doesn't yet exist)

	bool bChecked = status == Status::Taken;
	QVector<QSharedPointer<GoShoppingListItem>>& rOldList = bChecked ? m_SortedListUnchecked : m_SortedListChecked;
	QVector<QSharedPointer<GoShoppingListItem>>& rNewList = bChecked ? m_SortedListChecked : m_SortedListUnchecked;
	GoShoppingListItemType type = bCompatible ? GoShoppingListItemType::Category_Header : GoShoppingListItemType::Incompatible_Items_Header;

	quint32 headerPos = getItemIndex(strCategory, type, rNewList);
	if(headerPos == (quint32)rNewList.size())
	{
		{
			recipebook::RBDataReadHandle handle(rRBDataHandler);

			quint32 posInSortOrder = m_pSortOrder->getIndex(handle.data().getCategory(strCategory));

			int nextPos = getNextItemOfType(rNewList, type, 0);
			bool bFound = false;
			while(nextPos < rNewList.size())
			{
				const Category& rNextCategory = handle.data().getCategory(rNewList.at(nextPos)->getName());
				if(m_pSortOrder->getIndex(rNextCategory) > posInSortOrder)
				{
					bFound = true;
					break;
				}

				nextPos = getNextItemOfType(rNewList, type, nextPos+1);
			}
			headerPos = nextPos;

			if(!bFound && type == GoShoppingListItemType::Category_Header)
			{
				headerPos = getNextItemOfType(rNewList, GoShoppingListItemType::Incompatible_Items_Header, 0);
			}
		}

		quint32 delta = bChecked ? 0 : m_SortedListChecked.size();
		if(pCallback)
		{
			pCallback->beginInsertItem(headerPos + delta);
		}

		QSharedPointer<GoShoppingListItem> spHeader(new GoShoppingListItem(type, strCategory));
		rNewList.insert(headerPos, spHeader);
		if(currentItemPos >= headerPos + delta)
		{
			++currentItemPos;
		}

		if(pCallback)
		{
			pCallback->endInsertItem();
		}
	}

	quint32 newItemPos = headerPos + 1;
	for(; newItemPos < (quint32)rNewList.size(); ++newItemPos)
	{
		QSharedPointer<GoShoppingListItem> spCurrent = rNewList.at(newItemPos);
		if(spCurrent->getType() != GoShoppingListItemType::IngredientListItem
		   || helper::lessThan(rItem.getElementId(), spCurrent->getElementId()))
		{
			break;
		}
	}

	// Move item to new position
	if(pCallback)
	{
		quint32 delta = bChecked ? 0 : m_SortedListChecked.size();
		pCallback->beginMoveItem(currentItemPos, newItemPos + delta);
	}
	quint32 currentItemPosInList = rItem.getStatus() == Status::None ? currentItemPos - m_SortedListChecked.size() : currentItemPos;
	QSharedPointer<GoShoppingListItem> spItem = rOldList.at(currentItemPosInList);
	rOldList.removeAt(currentItemPosInList);
	rNewList.insert(newItemPos, spItem);
	if(pCallback)
	{
		pCallback->endMoveItem();
	}

	// Update status
	{
		recipebook::RBDataWriteHandle handle(rRBDataHandler);
		rItem.updateStatus(status);
	}
	if(pCallback)
	{
		quint32 delta = bChecked ? 0 : m_SortedListChecked.size();
		pCallback->itemChanged(newItemPos + delta);
	}
}
