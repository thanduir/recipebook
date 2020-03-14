#ifndef RECIPEBOOK_ITEMSUTIL_H
#define RECIPEBOOK_ITEMSUTIL_H

#include <QException>
#include <QSharedPointer>
#include <QString>

namespace recipebook::internal
{
    template<class T> bool exists(const QString strName, QVector<QSharedPointer<T>> allItems)
    {
        for(QSharedPointer<T> spItem : qAsConst(allItems))
        {
            if(spItem->getName() == strName)
            {
                return true;
            }
        }
        return false;
    }

    template<class T> void remove(const T& rItem, QVector<QSharedPointer<T>>& allItems)
    {
        for(int i = 0; i < allItems.size(); ++i)
        {
            if(allItems[i]->getName() == rItem.getName())
            {
                allItems.remove(i);
                return;
            }
        }
    }

    template<class T> QStringList getAllNames(QVector<QSharedPointer<T>> allItems, bool bSorted = true)
    {
        QStringList list;
        for(QSharedPointer<T> spItem : qAsConst(allItems))
        {
            list.append(spItem->getName());
        }
        if(bSorted)
        {
            // TODO: Does this sort the way i want it to?
            // Otherwise: std::sort(entryList.begin(), entryList.end(), collator), where collator is a QCollator
            list.sort(Qt::CaseInsensitive);
        }
        return list;
    }

    template<class T> const T& getItemConst(QString strName, const QVector<QSharedPointer<T>>& allItems)
    {
        for(QSharedPointer<T> spItem : qAsConst(allItems))
        {
            if(spItem->getName() == strName)
            {
                return *spItem.get();
            }
        }

        throw QException();
    }

    template<class T> T& getItem(QString strName, QVector<QSharedPointer<T>> allItems)
    {
        for(QSharedPointer<T> spItem : qAsConst(allItems))
        {
            if(spItem->getName() == strName)
            {
                return *spItem.get();
            }
        }

        throw QException();
    }
    
    template<class T, typename Func> 
    T& addItem(QString strName, QVector<QSharedPointer<T>>& allItems, Func constructNewItem)
    {
        for(QSharedPointer<T> spItem : qAsConst(allItems))
        {
            if(spItem->getName() == strName)
            {
                throw QException();
            }
        }

        QSharedPointer<T> spNewItem(constructNewItem());
        allItems.append(spNewItem);

        return *spNewItem.get();
    }
}

#endif
