#include "data/Amount.h"
#include <QException>


constexpr float valueSteps[] = { 0, 0.1f, 0.2f, 0.25f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f,
								 1.0f, 1.25f, 1.5f, 1.75f,
								 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f,
								 10.0f, 12.5f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f,
								 20.0f, 25.0f, 30.0f, 35.0f, 40.0f, 45.0f, 50.0f, 60.0f, 70.0f, 80.0f, 90.0f,
								 100.0f, 110.0f, 120.0f, 125.0f, 130.0f, 140.0f, 150.0f, 160.0f, 170.0f, 180.0f, 190.0f,
								 200.0f, 225.0f, 250.0f, 275.0f,
								 300.0f, 350.0f, 400.0f, 450.0f, 500.0f, 550.0f,
								 600.0f, 750.0f, 1000.0f, 1500.0f, 2000.0f, 3000.0f, 4000.0f, 5000.0f, 10000.0f };
constexpr int valueStepsCount = sizeof(valueSteps) / sizeof(valueSteps[0]);

void recipebook::Amount::decreaseQuantityMin()
{
	float quanitityMin = getQuantityMin();
	setQuantityMin(getChangedAmount(quanitityMin, false));
}

void recipebook::Amount::increaseQuantityMin()
{
	float quanitityMin = getQuantityMin();
	setQuantityMin(getChangedAmount(quanitityMin, true));
}

void recipebook::Amount::decreaseQuantityMax()
{
	if(isRange())
	{
		float quanitityMax = getQuantityMax();
		setQuantityMax(getChangedAmount(quanitityMax, false));
	}
}

void recipebook::Amount::increaseQuantityMax()
{
	if(isRange())
	{
		float quanitityMax = getQuantityMax();
		setQuantityMax(getChangedAmount(quanitityMax, true));
	}
}

float recipebook::Amount::getChangedAmount(float quantity, bool bIncrease)
{	
	if(getUnit() == Unit::Unitless)
	{
		return 0.0f;
	}

	const float* fPos = std::lower_bound(valueSteps, valueSteps + valueStepsCount, quantity);
    int position = static_cast<int>(fPos - valueSteps);
    if(bIncrease)
	{
		if(position == valueStepsCount - 1)
		{
			return quantity;
		}
		return valueSteps[position + 1];
	}
	else
	{
		if(position == 0)
		{
			return 0.0f;
		}
		return valueSteps[position - 1];
	}
}

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

			if(isRange() && !other.isRange())
			{
				float value = getQuantityMax() + other.getQuantityMin();
				setQuantityMax(value);
			}
			else if(isRange() && other.isRange())
			{
				float value = getQuantityMax() + other.getQuantityMax();
				setQuantityMax(value);
			}
			else if(!isRange() && other.isRange())
			{
				setIsRange(true);
				float value = getQuantityMin() + other.getQuantityMax() - other.getQuantityMin();
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

bool recipebook::Amount::operator==(const Amount& rOther) const
{
	if(m_Unit != rOther.m_Unit)
	{
		return false;
	}

	if(m_bRange != rOther.m_bRange)
	{
		return false;
	}

	if(m_fQuantityMin != rOther.m_fQuantityMin)
	{
		return false;
	}

	if(m_fQuantityMax != rOther.m_fQuantityMax)
	{
		return false;
	}

	return true;
}