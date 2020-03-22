#ifndef RECIPEBOOK_SERIALIZATION_HELPER_STRINGCONVERTER_H
#define RECIPEBOOK_SERIALIZATION_HELPER_STRINGCONVERTER_H

#include <QString>
#include "../../data/Size.h"
#include "../../data/Status.h"
#include "../../data/Unit.h"

namespace recipebook::serialization::helper
{
    QString convertSize(Size size);
    Size convertSize(QString strSize);

    QString convertStatus(Status status);
    Status convertStatus(QString strStatus);

    QString convertUnit(Unit unit);
    Unit convertUnit(QString strUnit);
}

#endif
