#include "data/Amount.h"
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

void recipebook::Amount::add(const Amount& other)
{
	if(!canBeAddedUp(*this, other))
	{
		throw QException();
	}

	switch(m_Unit)
	{
		case Unit::Count:
		case Unit::Dessertspoon:
		case Unit::Teaspoon:
		case Unit::Unitless:
		{
			setQuantityMin(getQuantityMin() + other.getQuantityMin());
			if(isRange() || other.isRange())
			{
				setIsRange(true);
				float value = getQuantityMax();
				if(other.isRange())
				{
					value += other.getQuantityMax();
				}
				else
				{
					value += other.getQuantityMin();
				}
				setQuantityMax(value);
			}
			break;
		}

		case Unit::Kilogram:
		case Unit::Gram:
		{
			float value1 = getUnit() == Unit::Kilogram ? getQuantityMin() * 1000.0f : getQuantityMin();
			float value2 = other.getUnit() == Unit::Kilogram ? other.getQuantityMin() * 1000.0f : other.getQuantityMin();
			float quantityMin = value1 + value2;

			float quantityMax = 0.0f;
			if(isRange() || other.isRange())
			{
				setIsRange(true);
				float value = getUnit() == Unit::Kilogram ? getQuantityMax() * 1000.0f : getQuantityMax();
				if(other.isRange())
				{
					value += other.getUnit() == Unit::Kilogram ? other.getQuantityMax() * 1000.0f : other.getQuantityMax();
				}
				else
				{
					value += other.getUnit() == Unit::Kilogram ? other.getQuantityMin() * 1000.0f : other.getQuantityMin();
				}
			}

			if(quantityMin >= 1000.0f)
			{
				setQuantityMin(quantityMin / 1000.0f);
				if(isRange())
				{
					setQuantityMax(quantityMax / 1000.0f);
				}
				setUnit(Unit::Kilogram);
			}
			else
			{
				setQuantityMin(quantityMin);
				if(isRange())
				{
					setQuantityMax(quantityMax);
				}
				setUnit(Unit::Gram);
			}
			break;
		}

		case Unit::Liter:
		case Unit::Deciliter:
		case Unit::Milliliter:
		{
			auto convertValue = [](Unit unit, float value) -> float
			{
				if(unit == Unit::Liter)
				{
					return value * 1000.0f;
				}
				else if(unit == Unit::Deciliter)
				{
					return value * 100.0f;
				}
				else
				{
					return value;
				}
			};

			float value1 = convertValue(getUnit(), getQuantityMin());
			float value2 = convertValue(other.getUnit(), other.getQuantityMin());
			float quantityMin = value1 + value2;

			float quantityMax = 0.0f;
			if(isRange() || other.isRange())
			{
				setIsRange(true);
				float value = convertValue(getUnit(), getQuantityMax());
				if(other.isRange())
				{
					value += convertValue(other.getUnit(), other.getQuantityMax());
				}
				else
				{
					value += convertValue(other.getUnit(), other.getQuantityMin());
				}
			}

			if(quantityMin >= 1000.0f)
			{
				setQuantityMin(quantityMin / 1000.0f);
				if(isRange())
				{
					setQuantityMax(quantityMax / 1000.0f);
				}
				setUnit(Unit::Liter);
			}
			else if(quantityMin >= 10.0f)
			{
				setQuantityMin(quantityMin / 100.0f);
				if(isRange())
				{
					setQuantityMax(quantityMax / 100.0f);
				}
				setUnit(Unit::Deciliter);
			}
			else
			{
				setQuantityMin(quantityMin);
				if(isRange())
				{
					setQuantityMax(quantityMax);
				}
				setUnit(Unit::Milliliter);
			}
			break;
		}

		default:
			throw QException();
	}
}

bool recipebook::Amount::canBeAddedUp(const Amount& m1, const Amount& m2)
{
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
