#include "Status.h"
#include <QSharedPointer>

QStringList recipebook::getAllStatusNamesUI()
{
    static QSharedPointer<QStringList> spList;
    if(spList == nullptr)
    {
        spList = QSharedPointer<QStringList>::create();
        // TODO: translations! -> Possibly move this somewhere else?
        spList->append("None");
        spList->append("Taken");
    }

    return *spList.get();
}
