#ifndef RECIPEBOOK_UNIT_H
#define RECIPEBOOK_UNIT_H

namespace recipebook
{
    enum class Unit
    {
        Count = 0,

        Kilogram,
        Gram,

        Liter,
        Deciliter,
        Milliliter,

        Dessertspoon,
        Teaspoon,

        Unitless
    };

    float getDefaultQuantity(Unit unit);
}

#endif
