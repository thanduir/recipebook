#include "uistringconverter.h"

UIStringConverter::UIStringConverter()
{
	m_strNoAlternativesGroup = tr("-");

	// Status

	const QString c_strStatusNone = tr("None");
	const QString c_strStatusTaken = tr("Taken");

	m_StatusToName.append(c_strStatusNone);
	m_StatusToName.append(c_strStatusTaken);

	// Size

	const QString c_strSizeSmall = tr("small");
	const QString c_strSizeNormal = tr("normal");
	const QString c_strSizeLarge = tr("large");

	m_SizeToName.append(c_strSizeSmall);
	m_SizeToName.append(c_strSizeNormal);
	m_SizeToName.append(c_strSizeLarge);

	const QString c_strSizeUnitlessSmall = tr("little");
	const QString c_strSizeUnitlessNormal = tr("normal");
	const QString c_strSizeUnitlessLarge = tr("plenty");

	m_SizeToNameUnitless.append(c_strSizeUnitlessSmall);
	m_SizeToNameUnitless.append(c_strSizeUnitlessNormal);
	m_SizeToNameUnitless.append(c_strSizeUnitlessLarge);

	// Unit

	const QString c_strUnitCount = tr("Count");
	const QString c_strUnitKilogram = tr("Kilogram");
	const QString c_strUnitGram = tr("Gram");
	const QString c_strUnitLiter = tr("Liter");
	const QString c_strUnitDeciliter = tr("Deciliter");
	const QString c_strUnitMilliliter = tr("Milliliter");
	const QString c_strUnitDessertspoon = tr("Dessertspoon");
	const QString c_strUnitTeaspoon = tr("Teaspoon");
	const QString c_strUnitUnitless = tr("Unitless");

	m_UnitToName.append(c_strUnitCount);
	m_UnitToName.append(c_strUnitKilogram);
	m_UnitToName.append(c_strUnitGram);
	m_UnitToName.append(c_strUnitLiter);
	m_UnitToName.append(c_strUnitDeciliter);
	m_UnitToName.append(c_strUnitMilliliter);
	m_UnitToName.append(c_strUnitDessertspoon);
	m_UnitToName.append(c_strUnitTeaspoon);
	m_UnitToName.append(c_strUnitUnitless);

	const QString c_strUnitCountShort = tr("Piece");
	const QString c_strUnitKilogramShort = tr("kg");
	const QString c_strUnitGramShort = tr("g");
	const QString c_strUnitLiterShort = tr("L");
	const QString c_strUnitDeciliterShort = tr("dl");
	const QString c_strUnitMilliliterShort = tr("ml");
	const QString c_strUnitDessertspoonShort = tr("ds");
	const QString c_strUnitTeaspoonShort = tr("ts");
	const QString c_strUnitUnitlessShort = tr("");

	m_UnitToShortName.append(c_strUnitCountShort);
	m_UnitToShortName.append(c_strUnitKilogramShort);
	m_UnitToShortName.append(c_strUnitGramShort);
	m_UnitToShortName.append(c_strUnitLiterShort);
	m_UnitToShortName.append(c_strUnitDeciliterShort);
	m_UnitToShortName.append(c_strUnitMilliliterShort);
	m_UnitToShortName.append(c_strUnitDessertspoonShort);
	m_UnitToShortName.append(c_strUnitTeaspoonShort);
	m_UnitToShortName.append(c_strUnitUnitlessShort);

	const QString c_strUnitCountPostfix = tr("x");
	const QString c_strUnitKilogramPostfix = tr("kg");
	const QString c_strUnitGramPostfix = tr("g");
	const QString c_strUnitLiterPostfix = tr("L");
	const QString c_strUnitDeciliterPostfix = tr("dl");
	const QString c_strUnitMilliliterPostfix = tr("ml");
	const QString c_strUnitDessertspoonPostfix = tr("ds");
	const QString c_strUnitTeaspoonPostfix = tr("ts");
	const QString c_strUnitUnitlessPostfix = tr("");

	m_UnitToPostfixes.append(c_strUnitCountPostfix);
	m_UnitToPostfixes.append(c_strUnitKilogramPostfix);
	m_UnitToPostfixes.append(c_strUnitGramPostfix);
	m_UnitToPostfixes.append(c_strUnitLiterPostfix);
	m_UnitToPostfixes.append(c_strUnitDeciliterPostfix);
	m_UnitToPostfixes.append(c_strUnitMilliliterPostfix);
	m_UnitToPostfixes.append(c_strUnitDessertspoonPostfix);
	m_UnitToPostfixes.append(c_strUnitTeaspoonPostfix);
	m_UnitToPostfixes.append(c_strUnitUnitlessPostfix);

	// SortedShoppingListOrdering

	const QString c_strListOrderingCombined = tr("Combined");
	const QString c_strListOrderingSeparateTakenItems = tr("SeparateTakenItems");

	m_ShoppingListOrderingToName.append(c_strListOrderingCombined);
	m_ShoppingListOrderingToName.append(c_strListOrderingSeparateTakenItems);
}

QString UIStringConverter::getStringNoAlternativesGroup() const
{
	return m_strNoAlternativesGroup;
}

QString UIStringConverter::convertSize(recipebook::Size size, recipebook::Unit unit) const
{
	if(unit == recipebook::Unit::Unitless)
	{
		return m_SizeToNameUnitless[(int)size];
	}

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
	
	for(int i = 0; i < m_SizeToNameUnitless.size(); ++i)
	{
		if(m_SizeToNameUnitless[i] == strSize)
		{
			return (recipebook::Size)i;
		}
	}

	return recipebook::Size::Normal;
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

QString UIStringConverter::getShoppingListOrderingNameCombined() const
{
	return convertShoppingListOrdering(recipebook::SortedShoppingListOrdering::Combined);
}

QString UIStringConverter::getShoppingListOrderingNameSeparate() const
{
	return convertShoppingListOrdering(recipebook::SortedShoppingListOrdering::SeparateTakenItems);
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
	return strAmount + strUnitText;
}
