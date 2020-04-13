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
        // TODO: I should probably use this (or something similar) also for the exist-Methods and other comparisons (where appropriate)!
        inline int lessThan(QString strFirst, QString strSecond)
        {
            // TODO(phiwid): Better compare method?
            return strFirst.toLower().compare(strSecond.toLower(), Qt::CaseInsensitive) < 0;
        }

        template<class T> 
        int findItem(QString strName, QVector<QSharedPointer<T>> allItems)
        {
            for(int i = 0; i < allItems.size(); ++i)
            {
                if(allItems[i]->getName() == strName)
                {
                    return i;
                }
            }

            return -1;
        }

        template<class T> 
        typename QVector<QSharedPointer<T>>::const_iterator findItemSorted(QString strName, const QVector<QSharedPointer<T>>& allItems)
        {
            auto comp = [](const QSharedPointer<T>& rItem, const QString& strName) -> bool
            {
                return recipebook::internal::helper::lessThan(rItem->getName(), strName);
            };

            return std::lower_bound(allItems.begin(), allItems.end(), strName, comp);
        }

        template<class T> 
        typename QVector<QSharedPointer<T>>::iterator findItemSorted(QString strName, QVector<QSharedPointer<T>>& allItems)
        {
            auto comp = [](const QSharedPointer<T>& rItem, const QString& strName) -> bool
            {
                return recipebook::internal::helper::lessThan(rItem->getName(), strName);
            };

            return std::lower_bound(allItems.begin(), allItems.end(), strName, comp);
        }
    }

    namespace sorted
    {
        template<class T> 
        void moveForNewName(T& rItem, const QString strNewName, QVector<QSharedPointer<T>>& allItems)
        {
            auto iterOldPos = helper::findItemSorted(rItem.getName(), allItems);
            if(iterOldPos != allItems.end() && (*iterOldPos)->getName() == rItem.getName())
            {
                QSharedPointer<T> savedItem = *iterOldPos;
                allItems.erase(iterOldPos);

                auto iterNewPos = helper::findItemSorted(strNewName, allItems);
                if(iterNewPos != allItems.end() && (*iterNewPos)->getName() == rItem.getName())
                {
                    throw QException();
                }
                allItems.insert(iterNewPos, savedItem);
            }
        }

        template<class T>
        bool exists(const QString strName, QVector<QSharedPointer<T>> allItems)
        {
            auto iter = helper::findItemSorted(strName, allItems);
            return iter != allItems.end() && (*iter)->getName() == strName;
        }

        template<class T> 
        void remove(const T& rItem, QVector<QSharedPointer<T>>& allItems)
        {
            auto iter = helper::findItemSorted(rItem.getName(), allItems);
            if(iter != allItems.end() && (*iter)->getName() == rItem.getName())
            {
                allItems.erase(iter);
            }
        }

        template<class T> 
        T& getItem(QString strName, QVector<QSharedPointer<T>> allItems)
        {
            auto iter = helper::findItemSorted(strName, allItems);
            if(iter != allItems.end() && (*iter)->getName() == strName)
            {
                return *(*iter).get();
            }

            throw QException();
        }

        template<class T, typename Func> 
        T& addItem(QString strName, QVector<QSharedPointer<T>>& allItems, Func constructNewItem)
        {
            auto iter = helper::findItemSorted(strName, allItems);
            if(iter != allItems.end() && (*iter)->getName() == strName)
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
        bool exists(const QString strName, QVector<QSharedPointer<T>> allItems)
        {
            return helper::findItem(strName, allItems) >= 0;
        }

        template<class T> 
        int find(const QString strName, QVector<QSharedPointer<T>> allItems)
        {
            return helper::findItem(strName, allItems);
        }

        template<class T> 
        void remove(const T& rItem, QVector<QSharedPointer<T>>& allItems)
        {
            int i = helper::findItem(rItem.getName(), allItems);
            if(i >= 0)
            {
                allItems.remove(i);
            }
        }

        template<class T> 
        T& getItem(QString strName, QVector<QSharedPointer<T>> allItems)
        {
            int i = helper::findItem(strName, allItems);
            if(i >= 0)
            {
                return *allItems[i].get();
            }

            throw QException();
        }

        template<class T, typename Func> 
        T& addItem(QString strName, int pos, QVector<QSharedPointer<T>>& allItems, Func constructNewItem)
        {
            if(helper::findItem(strName, allItems) >= 0)
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
