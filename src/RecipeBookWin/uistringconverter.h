#ifndef RECIPEBOOK_SERIALIZATION_HELPER_STRINGCONVERTER_H
#define RECIPEBOOK_SERIALIZATION_HELPER_STRINGCONVERTER_H

#include <QObject>
#include <QStringList>
#include <data/Size.h>
#include <data/Status.h>
#include <data/Unit.h>

class UIStringConverter : public QObject
{
    Q_OBJECT
public:
    UIStringConverter();

    QString getProvenanceEverywhere() const;

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

private:
    QString m_strProvenanceEverywhere;

    QStringList m_StatusToName;
    QStringList m_UnitToName;
    QStringList m_UnitToShortName;
    QStringList m_SizeToName;
};

#endif