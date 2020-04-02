#include "uistringconverter.h"

static constexpr char* c_strProvenanceEverywhere = "Everywhere";

static constexpr char* c_strSizeSmall			= "Small";
static constexpr char* c_strSizNormal			= "Normal";
static constexpr char* c_strSizeLarge			= "Large";

static constexpr char* c_strStatusNone			= "None";
static constexpr char* c_strStatusTaken			= "Taken";

static constexpr char* c_strUnitCount			= "Count";
static constexpr char* c_strUnitKilogram		= "Kilogram";
static constexpr char* c_strUnitGram			= "Gram";
static constexpr char* c_strUnitLiter			= "Liter";
static constexpr char* c_strUnitDeciliter		= "Deciliter";
static constexpr char* c_strUnitMilliliter		= "Milliliter";
static constexpr char* c_strUnitDessertspoon	= "Dessertspoon";
static constexpr char* c_strUnitTeaspoon		= "Teaspoon";
static constexpr char* c_strUnitUnitless		= "Unitless";

UIStringConverter::UIStringConverter()
{
	m_strProvenanceEverywhere = tr(c_strProvenanceEverywhere);

	// Status

	QString strStatusNone = tr(c_strStatusNone);
	m_StatusToName[(int) recipebook::Status::None] = strStatusNone;
	m_NameToStatus[strStatusNone] = (int)recipebook::Status::None;

	QString strStatusTaken = tr(c_strStatusTaken);
	m_StatusToName[(int) recipebook::Status::Taken] = strStatusTaken;
	m_NameToStatus[strStatusTaken] = (int)recipebook::Status::Taken;

	// Size

	QString strSizeSmall = tr(c_strStatusNone);
	m_SizeToName[(int) recipebook::Size::Small] = strSizeSmall;
	m_NameToSize[strSizeSmall] = (int)recipebook::Size::Small;

	QString strSizeNormal = tr(c_strStatusNone);
	m_SizeToName[(int) recipebook::Size::Normal] = strSizeNormal;
	m_NameToSize[strSizeNormal] = (int)recipebook::Size::Normal;

	QString strSizeLarge = tr(c_strStatusNone);
	m_SizeToName[(int) recipebook::Size::Large] = strSizeLarge;
	m_NameToSize[strSizeLarge] = (int)recipebook::Size::Large;

	// Unit

	QString strUnitCount = tr(c_strUnitCount);
	m_UnitToName[(int) recipebook::Unit::Count] = strUnitCount;
	m_NameToUnit[strUnitCount] = (int)recipebook::Unit::Count;

	QString strUnitKilogram = tr(c_strUnitKilogram);
	m_UnitToName[(int) recipebook::Unit::Kilogram] = strUnitKilogram;
	m_NameToUnit[strUnitKilogram] = (int)recipebook::Unit::Kilogram;

	QString strUnitGram = tr(c_strUnitGram);
	m_UnitToName[(int) recipebook::Unit::Gram] = strUnitGram;
	m_NameToUnit[strUnitGram] = (int)recipebook::Unit::Gram;

	QString strUnitLiter = tr(c_strUnitLiter);
	m_UnitToName[(int) recipebook::Unit::Liter] = strUnitLiter;
	m_NameToUnit[strUnitLiter] = (int)recipebook::Unit::Liter;

	QString strUnitDeciliter = tr(c_strUnitDeciliter);
	m_UnitToName[(int) recipebook::Unit::Deciliter] = strUnitDeciliter;
	m_NameToUnit[strUnitDeciliter] = (int)recipebook::Unit::Deciliter;

	QString strUnitMilliliter = tr(c_strUnitMilliliter);
	m_UnitToName[(int) recipebook::Unit::Milliliter] = strUnitMilliliter;
	m_NameToUnit[strUnitMilliliter] = (int)recipebook::Unit::Milliliter;

	QString strUnitDessertspoon = tr(c_strUnitDessertspoon);
	m_UnitToName[(int) recipebook::Unit::Dessertspoon] = strUnitDessertspoon;
	m_NameToUnit[strUnitDessertspoon] = (int)recipebook::Unit::Dessertspoon;

	QString strUnitTeaspoon = tr(c_strUnitTeaspoon);
	m_UnitToName[(int) recipebook::Unit::Teaspoon] = strUnitTeaspoon;
	m_NameToUnit[strUnitTeaspoon] = (int)recipebook::Unit::Teaspoon;

	QString strUnitUnitless = tr(c_strUnitUnitless);
	m_UnitToName[(int) recipebook::Unit::Unitless] = strUnitUnitless;
	m_NameToUnit[strUnitUnitless] = (int)recipebook::Unit::Unitless;
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
	if(!m_NameToSize.contains(strSize))
	{
		return recipebook::Size::Normal;
	}

	return (recipebook::Size)m_NameToSize[strSize];
}

QStringList UIStringConverter::getAllSizeNames() const
{
	return m_NameToSize.keys();
}

QString UIStringConverter::convertStatus(recipebook::Status status) const
{
	return m_StatusToName[(int)status];
}

recipebook::Status UIStringConverter::convertStatus(QString strStatus) const
{
	if(!m_NameToStatus.contains(strStatus))
	{
		return recipebook::Status::None;
	}

	return (recipebook::Status)m_NameToStatus[strStatus];
}

QStringList UIStringConverter::getAllStatusNames() const
{
	return m_NameToStatus.keys();
}

QString UIStringConverter::convertUnit(recipebook::Unit unit) const
{
	return m_UnitToName[(int)unit];
}

recipebook::Unit UIStringConverter::convertUnit(QString strUnit) const
{
	if(!m_NameToUnit.contains(strUnit))
	{
		return recipebook::Unit::Count;
	}

	return (recipebook::Unit)m_NameToUnit[strUnit];
}

QStringList UIStringConverter::getAllUnitNames() const
{
	return m_NameToUnit.keys();
}
