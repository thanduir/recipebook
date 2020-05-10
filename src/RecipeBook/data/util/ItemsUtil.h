#ifndef RECIPEBOOK_ITEMSUTIL_H
#define RECIPEBOOK_ITEMSUTIL_H

#include <QException>
#include <QSharedPointer>
#include <QString>
#include <algorithm>

namespace recipebook::internal
{
	namespace helper
	{
		inline int compare(QString strFirst, QString strSecond)
		{
			return QString::localeAwareCompare(strFirst.toLower(), strSecond.toLower());
		}

		inline bool lessThan(QString strFirst, QString strSecond)
		{
			return compare(strFirst, strSecond) < 0;
		}

		template<class T> 
		int findItem(QString strIdString, QVector<QSharedPointer<T>> allItems)
		{
			for(int i = 0; i < allItems.size(); ++i)
			{
				if(compare(allItems[i]->getIdString(), strIdString) == 0)
				{
					return i;
				}
			}

			return -1;
		}

		template<class T> 
		typename QVector<QSharedPointer<T>>::const_iterator findItemSorted(QString strIdString, const QVector<QSharedPointer<T>>& allItems)
		{
			auto comp = [](const QSharedPointer<T>& rItem, const QString& strIdString) -> bool
			{
				return recipebook::internal::helper::lessThan(rItem->getIdString(), strIdString);
			};

			return std::lower_bound(allItems.begin(), allItems.end(), strIdString, comp);
		}

		template<class T> 
		typename QVector<QSharedPointer<T>>::iterator findItemSorted(QString strIdString, QVector<QSharedPointer<T>>& allItems)
		{
			auto comp = [](const QSharedPointer<T>& rItem, const QString& strIdString) -> bool
			{
				return recipebook::internal::helper::lessThan(rItem->getIdString(), strIdString);
			};

			return std::lower_bound(allItems.begin(), allItems.end(), strIdString, comp);
		}
	}

	namespace sorted
	{
		template<class T> 
		void moveForNewIdString(T& rItem, const QString strNewIdString, QVector<QSharedPointer<T>>& allItems)
		{
			auto iterOldPos = helper::findItemSorted(rItem.getIdString(), allItems);
			if(iterOldPos != allItems.end() && helper::compare((*iterOldPos)->getIdString(), rItem.getIdString()) == 0)
			{
				QSharedPointer<T> savedItem = *iterOldPos;
				allItems.erase(iterOldPos);

				auto iterNewPos = helper::findItemSorted(strNewIdString, allItems);
				if(iterNewPos != allItems.end() && helper::compare((*iterNewPos)->getIdString(), rItem.getIdString()) == 0)
				{
					throw QException();
				}
				allItems.insert(iterNewPos, savedItem);
			}
		}

		template<class T>
		bool exists(const QString strIdString, QVector<QSharedPointer<T>> allItems)
		{
			auto iter = helper::findItemSorted(strIdString, allItems);
			return iter != allItems.end() && helper::compare((*iter)->getIdString(), strIdString) == 0;
		}

		template<class T> 
		void remove(const QString strIdString, QVector<QSharedPointer<T>>& allItems)
		{
			auto iter = helper::findItemSorted(strIdString, allItems);
			if(iter != allItems.end() && helper::compare((*iter)->getIdString(), strIdString) == 0)
			{
				allItems.erase(iter);
			}
		}

		template<class T> 
		T& getItem(QString strIdString, QVector<QSharedPointer<T>> allItems)
		{
			auto iter = helper::findItemSorted(strIdString, allItems);
			if(iter != allItems.end() && helper::compare((*iter)->getIdString(), strIdString) == 0)
			{
				return *(*iter).get();
			}

			throw QException();
		}

		template<class T, typename Func> 
		T& addItem(QString strIdString, QVector<QSharedPointer<T>>& allItems, Func constructNewItem)
		{
			auto iter = helper::findItemSorted(strIdString, allItems);
			if(iter != allItems.end() && helper::compare((*iter)->getIdString(), strIdString) == 0)
			{
				throw QException();
			}

			QSharedPointer<T> spNewItem(constructNewItem());
			allItems.insert(iter, spNewItem);

			return *spNewItem.get();
		}
	}

	namespace unsorted
	{
		template<class T> 
		bool exists(const QString strIdString, QVector<QSharedPointer<T>> allItems)
		{
			return helper::findItem(strIdString, allItems) >= 0;
		}

		template<class T> 
		int find(const QString strIdString, QVector<QSharedPointer<T>> allItems)
		{
			return helper::findItem(strIdString, allItems);
		}

		template<class T> 
		void remove(const QString strIdString, QVector<QSharedPointer<T>>& allItems)
		{
			int i = helper::findItem(strIdString, allItems);
			if(i >= 0)
			{
				allItems.remove(i);
			}
		}

		template<class T> 
		T& getItem(QString strIdString, QVector<QSharedPointer<T>> allItems)
		{
			int i = helper::findItem(strIdString, allItems);
			if(i >= 0)
			{
				return *allItems[i].get();
			}

			throw QException();
		}

		template<class T, typename Func> 
		T& addItem(QString strIdString, int pos, QVector<QSharedPointer<T>>& allItems, Func constructNewItem)
		{
			if(helper::findItem(strIdString, allItems) >= 0)
			{
				throw QException();
			}

			QSharedPointer<T> spNewItem(constructNewItem());
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
