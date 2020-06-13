#ifndef RECIPEBOOK_SERIALIZATION_HELPER_STRINGCONVERTER_H
#define RECIPEBOOK_SERIALIZATION_HELPER_STRINGCONVERTER_H

#include <QObject>
#include <QStringList>
#include <data/Size.h>
#include <data/Status.h>
#include <data/Unit.h>
#include <data/Amount.h>
#include <data/SortedShoppingListOrdering.h>

class UIStringConverter : public QObject
{
	Q_OBJECT

public:
	UIStringConverter();

	QString getStringNoAlternativesGroup() const;

	QString convertSize(recipebook::Size size, recipebook::Unit unit) const;
	recipebook::Size convertSize(QString strSize) const;

	QString convertStatus(recipebook::Status status) const;
	recipebook::Status convertStatus(QString strStatus) const;

	QString convertUnit(recipebook::Unit unit) const;
	QString getUnitShortName(recipebook::Unit unit) const;
	QString getUnitPostfix(recipebook::Unit unit) const;
	recipebook::Unit convertUnit(QString strUnit) const;

	QString convertShoppingListOrdering(recipebook::SortedShoppingListOrdering ordering) const;
	recipebook::SortedShoppingListOrdering convertShoppingListOrdering(QString strOrdering) const;

	QString formatAmount(recipebook::Amount amount, bool bUsePostfix = true) const;

public slots:
	QStringList getAllUnitNames() const;
	QString getUnitShortName(quint32 uiIndex) const { return getUnitShortName((recipebook::Unit)uiIndex); }

	QString getSizeName(quint32 uiIndex, quint32 uiUnit) const { return convertSize((recipebook::Size)uiIndex, (recipebook::Unit)uiUnit); }
	quint32 getSizeIndex(QString strName) const { return (quint32)convertSize(strName); }
	quint32 getSizesCount() const { return m_SizeToName.size(); }

	QString getShoppingListOrderingNameCombined() const;
	QString getShoppingListOrderingNameSeparate() const;

private:
	QString m_strNoAlternativesGroup;

	QStringList m_StatusToName;
	QStringList m_UnitToName;
	QStringList m_UnitToShortName;
	QStringList m_UnitToPostfixes;
	QStringList m_SizeToName;
	QStringList m_SizeToNameUnitless;
	QStringList m_ShoppingListOrderingToName;
};

#endif