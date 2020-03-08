#include "Amount.h"

void recipebook::Amount::setIsRange(bool bIsRange)
{
    if(bIsRange == m_bRange)
    {
        return;
    }

    if(bIsRange)
    {
        m_fQuantityMax = m_fQuantityMin;
    }
    else
    {
        m_fQuantityMax = 0.0f;
    }
}
