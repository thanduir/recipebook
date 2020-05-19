#include "uistringconverter.h"

static constexpr char* c_strNoAlternativesGroup		= "-";

static constexpr char* c_strStatusNone				= "None";
static constexpr char* c_strStatusTaken				= "Taken";

static constexpr char* c_strSizeSmall				= "Small";
static constexpr char* c_strSizNormal				= "Normal";
static constexpr char* c_strSizeLarge				= "Large";

static constexpr char* c_strUnitCount				= "Count";
static constexpr char* c_strUnitKilogram			= "Kilogram";
static constexpr char* c_strUnitGram				= "Gram";
static constexpr char* c_strUnitLiter				= "Liter";
static constexpr char* c_strUnitDeciliter			= "Deciliter";
static constexpr char* c_strUnitMilliliter			= "Milliliter";
static constexpr char* c_strUnitDessertspoon		= "Dessertspoon";
static constexpr char* c_strUnitTeaspoon			= "Teaspoon";
static constexpr char* c_strUnitUnitless			= "Unitless";

static constexpr char* c_strUnitCountShort			= "Piece";
static constexpr char* c_strUnitKilogramShort		= "kg";
static constexpr char* c_strUnitGramShort			= "g";
static constexpr char* c_strUnitLiterShort			= "L";
static constexpr char* c_strUnitDeciliterShort		= "dl";
static constexpr char* c_strUnitMilliliterShort		= "ml";
static constexpr char* c_strUnitDessertspoonShort	= "ds";
static constexpr char* c_strUnitTeaspoonShort		= "ts";
static constexpr char* c_strUnitUnitlessShort		= "";

static constexpr char* c_strUnitCountPostfix		= "";
static constexpr char* c_strUnitKilogramPostfix		= "kg";
static constexpr char* c_strUnitGramPostfix			= "g";
static constexpr char* c_strUnitLiterPostfix		= "L";
static constexpr char* c_strUnitDeciliterPostfix	= "dl";
static constexpr char* c_strUnitMilliliterPostfix	= "ml";
static constexpr char* c_strUnitDessertspoonPostfix = "ds";
static constexpr char* c_strUnitTeaspoonPostfix		= "ts";
static constexpr char* c_strUnitUnitlessPostfix		= "";

static constexpr char* c_strListOrderingCombined			= "Combined";
static constexpr char* c_strListOrderingSeparateTakenItems	= "SeparateTakenItems";

UIStringConverter::UIStringConverter()
{
	m_strNoAlternativesGroup = tr(c_strNoAlternativesGroup);

	// Status

	m_StatusToName.append(tr(c_strStatusNone));
	m_StatusToName.append(tr(c_strStatusTaken));

	// Size

	m_SizeToName.append(tr(c_strSizeSmall));
	m_SizeToName.append(tr(c_strSizNormal));
	m_SizeToName.append(tr(c_strSizeLarge));

	// Unit

	m_UnitToName.append(tr(c_strUnitCount));
	m_UnitToName.append(tr(c_strUnitKilogram));
	m_UnitToName.append(tr(c_strUnitGram));
	m_UnitToName.append(tr(c_strUnitLiter));
	m_UnitToName.append(tr(c_strUnitDeciliter));
	m_UnitToName.append(tr(c_strUnitMilliliter));
	m_UnitToName.append(tr(c_strUnitDessertspoon));
	m_UnitToName.append(tr(c_strUnitTeaspoon));
	m_UnitToName.append(tr(c_strUnitUnitless));

	m_UnitToShortName.append(tr(c_strUnitCountShort));
	m_UnitToShortName.append(tr(c_strUnitKilogramShort));
	m_UnitToShortName.append(tr(c_strUnitGramShort));
	m_UnitToShortName.append(tr(c_strUnitLiterShort));
	m_UnitToShortName.append(tr(c_strUnitDeciliterShort));
	m_UnitToShortName.append(tr(c_strUnitMilliliterShort));
	m_UnitToShortName.append(tr(c_strUnitDessertspoonShort));
	m_UnitToShortName.append(tr(c_strUnitTeaspoonShort));
	m_UnitToShortName.append(tr(c_strUnitUnitlessShort));

	m_UnitToPostfixes.append(tr(c_strUnitCountPostfix));
	m_UnitToPostfixes.append(tr(c_strUnitKilogramPostfix));
	m_UnitToPostfixes.append(tr(c_strUnitGramPostfix));
	m_UnitToPostfixes.append(tr(c_strUnitLiterPostfix));
	m_UnitToPostfixes.append(tr(c_strUnitDeciliterPostfix));
	m_UnitToPostfixes.append(tr(c_strUnitMilliliterPostfix));
	m_UnitToPostfixes.append(tr(c_strUnitDessertspoonPostfix));
	m_UnitToPostfixes.append(tr(c_strUnitTeaspoonPostfix));
	m_UnitToPostfixes.append(tr(c_strUnitUnitlessPostfix));	

	// SortedShoppingListOrdering

	m_ShoppingListOrderingToName.append(tr(c_strListOrderingCombined));
	m_ShoppingListOrderingToName.append(tr(c_strListOrderingSeparateTakenItems));
}

QString UIStringConverter::getStringNoAlternativesGroup() const
{
	return m_strNoAlternativesGroup;
}

QString UIStringConverter::convertSize(recipebook::Size size) const
{
	return m_SizeToName[(int)size];
}

recipebook::Size UIStringConverter::convertSize(QString strSize) const
{
	for(int i = 0; i < m_SizeToName.size(); ++i)
	{
		if(m_SizeToName[i] == strSize)
		{
			return (recipebook::Size)i;
		}
	}
	
	return recipebook::Size::Normal;
}

QStringList UIStringConverter::getAllSizeNames() const
{
	return m_SizeToName;
}

QString UIStringConverter::convertStatus(recipebook::Status status) const
{
	return m_StatusToName[(int)status];
}

recipebook::Status UIStringConverter::convertStatus(QString strStatus) const
{
	for(int i = 0; i < m_StatusToName.size(); ++i)
	{
		if(m_StatusToName[i] == strStatus)
		{
			return (recipebook::Status)i;
		}
	}
	
	return recipebook::Status::None;
}

QStringList UIStringConverter::getAllStatusNames() const
{
	return m_StatusToName;
}

QString UIStringConverter::convertUnit(recipebook::Unit unit) const
{
	return m_UnitToName[(int)unit];
}

QString UIStringConverter::getUnitShortName(recipebook::Unit unit) const
{
	return m_UnitToShortName[(int)unit];
}

QString UIStringConverter::getUnitPostfix(recipebook::Unit unit) const
{
	return m_UnitToPostfixes[(int)unit];
}

recipebook::Unit UIStringConverter::convertUnit(QString strUnit) const
{
	for(int i = 0; i < m_UnitToName.size(); ++i)
	{
		if(m_UnitToName[i] == strUnit)
		{
			return (recipebook::Unit)i;
		}
	}
	
	return recipebook::Unit::Count;
}

QStringList UIStringConverter::getAllUnitNames() const
{
	return m_UnitToName;
}

QStringList UIStringConverter::getAllUnitShortNames() const
{
	return m_UnitToShortName;
}

QString UIStringConverter::convertShoppingListOrdering(recipebook::SortedShoppingListOrdering ordering) const
{
	return m_ShoppingListOrderingToName[(int)ordering];
}

recipebook::SortedShoppingListOrdering UIStringConverter::convertShoppingListOrdering(QString strOrdering) const
{
	for(int i = 0; i < m_ShoppingListOrderingToName.size(); ++i)
	{
		if(m_ShoppingListOrderingToName[i] == strOrdering)
		{
			return (recipebook::SortedShoppingListOrdering)i;
		}
	}
	
	return recipebook::SortedShoppingListOrdering::Combined;
}

QStringList UIStringConverter::getAllShoppingListOrderingNames() const
{
	return m_ShoppingListOrderingToName;
}

QString UIStringConverter::formatAmount(recipebook::Amount amount, bool bUsePostfix) const
{
	if(amount.getUnit() == recipebook::Unit::Unitless)
	{
		return "";
	}

	QString strAmount;
	if(amount.isRange())
	{
		strAmount = QString("%1-%2").arg(amount.getQuantityMin()).arg(amount.getQuantityMax());
	}
	else
	{
		strAmount = QString("%1").arg(amount.getQuantityMin());
	}
	QString strUnitText = bUsePostfix ? getUnitPostfix(amount.getUnit()) : getUnitShortName(amount.getUnit());
	if(!strUnitText.isEmpty())
	{
		strUnitText = " " + strUnitText;
	}
	return strAmount + strUnitText;
}
