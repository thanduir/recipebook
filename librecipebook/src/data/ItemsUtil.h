#ifndef RECIPEBOOK_ITEMSUTIL_H
#define RECIPEBOOK_ITEMSUTIL_H

#include <QException>
#include <QSharedPointer>
#include <algorithm>
#include "data/RBElementId.h"

namespace recipebook::internal
{
	namespace helper
	{
		template<class T> 
		int findItem(const RBElementId& idString, QVector<QSharedPointer<T>> allItems)
		{
			for(int i = 0; i < allItems.size(); ++i)
			{
				if(recipebook::helper::compare(allItems[i]->getElementId(), idString) == 0)
				{
					return i;
				}
			}

			return -1;
		}

		template<class T> 
		void sort(QVector<QSharedPointer<T>>& allItems)
		{
			auto comp = [](const QSharedPointer<T>& rItemFirst, const QSharedPointer<T>& rItemSecond) -> bool
			{
				return recipebook::helper::lessThan(rItemFirst->getElementId(), rItemSecond->getElementId());
			};

			std::sort(allItems.begin(), allItems.end(), comp);
		}

		template<class T> 
		typename QVector<QSharedPointer<T>>::const_iterator findItemSorted(const RBElementId& idString, const QVector<QSharedPointer<T>>& allItems)
		{
			auto comp = [](const QSharedPointer<T>& rItem, const RBElementId& idString) -> bool
			{
				return recipebook::helper::lessThan(rItem->getElementId(), idString);
			};

			return std::lower_bound(allItems.begin(), allItems.end(), idString, comp);
		}

		template<class T> 
		typename QVector<QSharedPointer<T>>::iterator findItemSorted(const RBElementId& idString, QVector<QSharedPointer<T>>& allItems)
		{
			auto comp = [](const QSharedPointer<T>& rItem, const RBElementId& idString) -> bool
			{
				return recipebook::helper::lessThan(rItem->getElementId(), idString);
			};

			return std::lower_bound(allItems.begin(), allItems.end(), idString, comp);
		}
	}

	namespace sorted
	{
		template<class T> 
		void moveForNewIdString(T& rItem, const RBElementId strNewIdString, QVector<QSharedPointer<T>>& allItems)
		{
			auto iterOldPos = helper::findItemSorted(rItem.getElementId(), allItems);
			if(iterOldPos != allItems.end())
			{
				if(recipebook::helper::compare((*iterOldPos)->getElementId(), rItem.getElementId()) == 0)
				{
					QSharedPointer<T> savedItem = *iterOldPos;
					allItems.erase(iterOldPos);

					auto iterNewPos = helper::findItemSorted(strNewIdString, allItems);
					if(iterNewPos != allItems.end() && recipebook::helper::compare((*iterNewPos)->getElementId(), rItem.getElementId()) == 0)
					{
						throw QException();
					}
					allItems.insert(iterNewPos, savedItem);
				}
			}
		}

		template<class T>
		bool exists(const RBElementId& idString, QVector<QSharedPointer<T>> allItems)
		{
			auto iter = helper::findItemSorted(idString, allItems);
			return iter != allItems.end() && recipebook::helper::compare((*iter)->getElementId(), idString) == 0;
		}

		template<class T> 
		void remove(const RBElementId& idString, QVector<QSharedPointer<T>>& allItems)
		{
			auto iter = helper::findItemSorted(idString, allItems);
			if(iter != allItems.end() && recipebook::helper::compare((*iter)->getElementId(), idString) == 0)
			{
				allItems.erase(iter);
			}
		}

		template<class T> 
		T& getItem(const RBElementId& idString, QVector<QSharedPointer<T>> allItems)
		{
			auto iter = helper::findItemSorted(idString, allItems);
			if(iter != allItems.end() && recipebook::helper::compare((*iter)->getElementId(), idString) == 0)
			{
				return *(*iter).get();
			}

			throw QException();
		}

		template<class T> 
		T& addItem(const RBElementId& idString, QVector<QSharedPointer<T>>& allItems, std::function<T*()> rConstructNewItem)
		{
			auto iter = helper::findItemSorted(idString, allItems);
			if(iter != allItems.end() && recipebook::helper::compare((*iter)->getElementId(), idString) == 0)
			{
				throw QException();
			}

			QSharedPointer<T> spNewItem(rConstructNewItem());
			allItems.insert(iter, spNewItem);

			return *spNewItem.get();
		}
	}

	namespace unsorted
	{
		template<class T> 
		bool exists(const RBElementId& idString, QVector<QSharedPointer<T>> allItems)
		{
			return helper::findItem(idString, allItems) >= 0;
		}

		template<class T> 
		int find(const RBElementId& idString, QVector<QSharedPointer<T>> allItems)
		{
			return helper::findItem(idString, allItems);
		}

		template<class T> 
		void remove(const RBElementId& idString, QVector<QSharedPointer<T>>& allItems)
		{
			int i = helper::findItem(idString, allItems);
			if(i >= 0)
			{
				allItems.remove(i);
			}
		}

		template<class T> 
		T& getItem(const RBElementId& idString, QVector<QSharedPointer<T>> allItems)
		{
			int i = helper::findItem(idString, allItems);
			if(i >= 0)
			{
				return *allItems[i].get();
			}

			throw QException();
		}

		template<class T> 
		T& addItem(const RBElementId& idString, int pos, QVector<QSharedPointer<T>>& allItems, std::function<T*()> rConstructNewItem)
		{
			if(helper::findItem(idString, allItems) >= 0)
			{
				throw QException();
			}

			QSharedPointer<T> spNewItem(rConstructNewItem());
			if(pos >= 0)
			{
				allItems.insert(pos, spNewItem);
			}
			else
			{
				allItems.append(spNewItem);
			}

			return *spNewItem.get();
		}
	}
}

#endif
