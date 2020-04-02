#ifndef RECIPEBOOK_SERIALIZATION_HELPER_STRINGCONVERTER_H
#define RECIPEBOOK_SERIALIZATION_HELPER_STRINGCONVERTER_H

#include <QObject>
#include <QHash>
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

private:
    QString m_strProvenanceEverywhere;

    QHash<int, QString> m_StatusToName;
    QHash<QString, int> m_NameToStatus;

    QHash<int, QString> m_UnitToName;
    QHash<QString, int> m_NameToUnit;

    QHash<int, QString> m_SizeToName;
    QHash<QString, int> m_NameToSize;
};

#endif