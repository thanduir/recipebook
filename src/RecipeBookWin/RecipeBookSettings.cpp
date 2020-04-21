#include "RecipeBookSettings.h"
#include <QCoreApplication>
#include <QSettings>
#include <QUuid>
#include "serialization/helper/StringConverter.h"

using namespace recipebook;

constexpr char* c_strAppInstanceUID     = "appInstanceUID";

constexpr char* c_strDefaultUnit        = "defaults/unit";
constexpr char* c_strDefaultNrPersons   = "defaults/nrpersons";
constexpr char* c_strDefaultCategory    = "defaults/category";
constexpr char* c_strDefaultSortOrder   = "defaults/sortorder";

// On windows this should save to HKEY_CURRENT_USER\Software\phwidmer.ch\RecipeBook

RecipeBookSettings::RecipeBookSettings()
{
	QCoreApplication::setOrganizationName("phwidmer.ch");
    QCoreApplication::setOrganizationDomain("phwidmer.ch");
    QCoreApplication::setApplicationName("RecipeBook");
}

QString RecipeBookSettings::getApplicationInstanceUID()
{
    QSettings settings;
    if(!settings.contains(c_strAppInstanceUID))
    {
        settings.setValue(c_strAppInstanceUID, QUuid::createUuid().toString(QUuid::WithoutBraces));
    }

    return settings.value(c_strAppInstanceUID).toString();
}

quint32 RecipeBookSettings::getDefaultUnit() const
{
    QSettings settings;
    if(!settings.contains(c_strDefaultUnit))
    {
        return 0;
    }

    return settings.value(c_strDefaultUnit).toUInt();
}

void RecipeBookSettings::setDefaultUnit(quint32 defaultUnit)
{
    QSettings settings;
    settings.setValue(c_strDefaultUnit, defaultUnit);
}

quint32 RecipeBookSettings::getDefaultRecipeNrPersons() const
{
    QSettings settings;
    return settings.value(c_strDefaultNrPersons, 2).toUInt();
}

void RecipeBookSettings::setDefaultRecipeNrPersons(quint32 nrPersons)
{
    QSettings settings;
    settings.setValue(c_strDefaultNrPersons, nrPersons);
}

QString RecipeBookSettings::getDefaultCategory() const
{
    QSettings settings;
    return settings.value(c_strDefaultCategory, "").toString();
}

void RecipeBookSettings::setDefaultCategory(QString strCategory)
{
    QSettings settings;
    settings.setValue(c_strDefaultCategory, strCategory);
}

QString RecipeBookSettings::getDefaultSortOrder() const
{
    QSettings settings;
    return settings.value(c_strDefaultSortOrder, "").toString();
}

void RecipeBookSettings::setDefaultSortOrder(QString strSortOrder)
{
    QSettings settings;
    settings.setValue(c_strDefaultSortOrder, strSortOrder);
}
