#ifndef RECIPEBOOK_SERIALIZATION_HELPER_STRINGCONVERTER_H
#define RECIPEBOOK_SERIALIZATION_HELPER_STRINGCONVERTER_H

#include <QObject>
#include <QStringList>
#include <data/Size.h>
#include <data/Status.h>
#include <data/Unit.h>
#include <data/SortedShoppingListOrdering.h>

class UIStringConverter : public QObject
{
	Q_OBJECT
public:
	UIStringConverter();

	QString getProvenanceEverywhere() const;

	QString getStringNoAlternativesGroup() const;

	QString convertSize(recipebook::Size size) const;
	recipebook::Size convertSize(QString strSize) const;
	QStringList getAllSizeNames() const;

	QString convertStatus(recipebook::Status status) const;
	recipebook::Status convertStatus(QString strStatus) const;
	QStringList getAllStatusNames() const;

	QString convertUnit(recipebook::Unit unit) const;
	recipebook::Unit convertUnit(QString strUnit) const;
	QStringList getAllUnitNames() const;
	QStringList getAllUnitShortNames() const;

	QString convertShoppingListOrdering(recipebook::SortedShoppingListOrdering ordering) const;
	recipebook::SortedShoppingListOrdering convertShoppingListOrdering(QString strOrdering) const;
	QStringList getAllShoppingListOrderingNames() const;

private:
	QString m_strProvenanceEverywhere;
	QString m_strNoAlternativesGroup;

	QStringList m_StatusToName;
	QStringList m_UnitToName;
	QStringList m_UnitToShortName;
	QStringList m_SizeToName;
	QStringList m_ShoppingListOrderingToName;
};

#endif