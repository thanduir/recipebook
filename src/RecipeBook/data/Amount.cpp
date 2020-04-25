#include "Amount.h"
#include <QException>

void recipebook::Amount::setIsRange(bool bIsRange)
{
	if(bIsRange == m_bRange)
	{
		return;
	}

	m_bRange = bIsRange;

	if(bIsRange)
	{
		m_fQuantityMax = m_fQuantityMin;
	}
	else
	{
		m_fQuantityMax = 0.0f;
	}
}

void recipebook::Amount::scaleAmount(float fFactor)
{
	if(getUnit() == Unit::Unitless || fFactor < 0.0f)
	{
		return;
	}

	setQuantityMin(getQuantityMin() * fFactor);
	if(isRange())
	{
		setQuantityMax(getQuantityMax() * fFactor);
	}
}

bool recipebook::Amount::canBeAddedUp(const Amount& m1, const Amount& m2)
{
	if(m1.isRange() || m2.isRange())
	{
		return false;
	}

	switch(m1.getUnit())
	{
		case Unit::Count:
			return m2.getUnit() == Unit::Count;

		case Unit::Kilogram:
		case Unit::Gram:
			return m2.getUnit() == Unit::Kilogram || m2.getUnit() == Unit::Gram;

		case Unit::Liter:
		case Unit::Deciliter:
		case Unit::Milliliter:
			return m2.getUnit() == Unit::Liter || m2.getUnit() == Unit::Deciliter || m2.getUnit() == Unit::Milliliter;

		case Unit::Dessertspoon:
			return m2.getUnit() == Unit::Dessertspoon;

		case Unit::Teaspoon:
			return m2.getUnit() == Unit::Teaspoon;

		case Unit::Unitless:
			return m2.getUnit() == Unit::Unitless;

		default:
			throw QException();
	}
}

void recipebook::Amount::addUp(const Amount& m1, const Amount& m2, Amount& rResult)
{
	if(!canBeAddedUp(m1, m2))
	{
		throw QException();
	}

	switch(m1.getUnit())
	{
		case Unit::Count:
		case Unit::Dessertspoon:
		case Unit::Teaspoon:
		case Unit::Unitless:
		{
			rResult.setQuantityMin(m1.getQuantityMin() + m2.getQuantityMin());
			rResult.setUnit(m1.getUnit());
			break;
		}

		case Unit::Kilogram:
		case Unit::Gram:
		{
			float value1 = m1.getUnit() == Unit::Kilogram ? m1.getQuantityMin() * 1000.0f : m1.getQuantityMin();
			float value2 = m2.getUnit() == Unit::Kilogram ? m2.getQuantityMin() * 1000.0f : m2.getQuantityMin();
			rResult.setQuantityMin(value1 + value2);

			if(rResult.getQuantityMin() >= 1000.0f)
			{
				rResult.setQuantityMin(rResult.getQuantityMin() / 1000.0f);
				rResult.setUnit(Unit::Kilogram);
			}
			else
			{
				rResult.setUnit(Unit::Gram);
			}
			break;
		}

		case Unit::Liter:
		case Unit::Deciliter:
		case Unit::Milliliter:
		{
			float value1 = m1.getQuantityMin();
			if(m1.getUnit() == Unit::Liter)
			{
				value1 *= 1000.0f;
			}
			else if(m1.getUnit() == Unit::Deciliter)
			{
				value1 *= 100.0f;
			}

			float value2 = m2.getQuantityMin();
			if(m2.getUnit() == Unit::Liter)
			{
				value2 *= 1000.0f;
			}
			else if(m2.getUnit() == Unit::Deciliter)
			{
				value2 *= 100.0f;
			}

			rResult.setQuantityMin(value1 + value2);
			if(rResult.getQuantityMin() >= 1000.0f)
			{
				rResult.setQuantityMin(rResult.getQuantityMin() / 1000.0f);
				rResult.setUnit(Unit::Liter);
			}
			else if(rResult.getQuantityMin() >= 10.0f)
			{
				rResult.setQuantityMin(rResult.getQuantityMin() / 100.0f);
				rResult.setUnit(Unit::Deciliter);
			}
			else
			{
				rResult.setUnit(Unit::Milliliter);
			}
			break;
		}

		default:
			throw QException();
	}
}
