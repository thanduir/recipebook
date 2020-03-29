#include "Size.h"
#include <QSharedPointer>

QStringList recipebook::getAllSizeNamesUI()
{
    static QSharedPointer<QStringList> spList;
    if(spList == nullptr)
    {
        spList = QSharedPointer<QStringList>::create();
        // TODO: translations! -> Possibly move this somewhere else?
        spList->append("Small");
        spList->append("Normal");
        spList->append("Large");
    }

    return *spList.get();
}
