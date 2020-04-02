#include "Unit.h"
#include <QSharedPointer>

float recipebook::getDefaultQuantity(Unit unit)
{
    switch(unit)
    {
        case Unit::Count:
            return 1.0f;

        case Unit::Kilogram:
        case Unit::Gram:
        case Unit::Liter:
        case Unit::Deciliter:
        case Unit::Milliliter:
        case Unit::Dessertspoon:
        case Unit::Teaspoon:
        case Unit::Unitless:
        default:
            return 0.0f;
    }
}
