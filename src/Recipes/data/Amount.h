#ifndef RECIPEBOOK_AMOUNT_H
#define RECIPEBOOK_AMOUNT_H

#include <QtGlobal>
#include "Unit.h"

namespace recipebook
{
    class Amount
    {
    public:
        Amount() {}

        Unit getUnit() const { return m_Unit; }
        void setUnit(Unit unit) { m_Unit = unit; }

        float getQuantityMin() const { return m_fQuantityMin; }
        void setQuantityMin(float quantity) { m_fQuantityMin = qMax(quantity, 0.0f); }

        float getQuantityMax() const { return m_fQuantityMax; }
        void setQuantityMax(float quantity) { m_fQuantityMax = qMax(quantity, 0.0f); }

        bool isRange() const { return m_bRange; }
        void setIsRange(bool bIsRange);

        void scaleAmount(float fFactor);

        static bool canBeAddedUp(const Amount& m1, const Amount& m2);
        static void addUp(const Amount& m1, const Amount& m2, Amount& rResult);

    private:
        Unit m_Unit          = Unit::Count;
        bool m_bRange        = false;
        float m_fQuantityMin = 1.0f;
        float m_fQuantityMax = 0.0f;
    };
}

#endif
