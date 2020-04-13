#include "uistringconverter.h"

static constexpr char* c_strProvenanceEverywhere	= "Everywhere";

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

UIStringConverter::UIStringConverter()
{
	m_strProvenanceEverywhere = tr(c_strProvenanceEverywhere);

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
}

QString UIStringConverter::getProvenanceEverywhere() const
{
	return m_strProvenanceEverywhere;
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
