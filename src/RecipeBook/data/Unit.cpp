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

QStringList recipebook::getAllUnitNamesUI()
{
    static QSharedPointer<QStringList> spList;
    if(spList == nullptr)
    {
        spList = QSharedPointer<QStringList>::create();

        // TODO: translations! -> Possibly move this somewhere else?
        spList->append("Count");
        spList->append("Kilogram");
        spList->append("Gram");
        spList->append("Liter");
        spList->append("Deciliter");
        spList->append("Milliliter");
        spList->append("Dessertspoon");
        spList->append("Teaspoon");
        spList->append("Unitless");
    }

    return *spList.get();
}
