#ifndef RECIPEBOOK_STATUS_H
#define RECIPEBOOK_STATUS_H

#include <QStringList>

namespace recipebook
{
    enum class Status
    {
        None = 0,
        Taken
    };

    QStringList getAllStatusNamesUI();
}

#endif
