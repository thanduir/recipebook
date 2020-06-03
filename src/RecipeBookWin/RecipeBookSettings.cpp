#include "RecipeBookSettings.h"
#include <QCoreApplication>
#include <QSettings>
#include <QStandardPaths>
#include <QUuid>
#include "serialization/helper/StringConverter.h"

using namespace recipebook;

constexpr char* c_strAppRBSaveFilename				= "/RecipeBook.json";

constexpr char* c_strAppInstanceUID					= "appInstanceUID";

constexpr char* c_strDefaultUnit					= "defaults/unit";
constexpr char* c_strDefaultNrPersons				= "defaults/nrpersons";
constexpr char* c_strDefaultCategory				= "defaults/category";
constexpr char* c_strDefaultSortOrder				= "defaults/sortorder";

constexpr char* c_strActiveSortOrderGoShopping		= "actives/sortorder_goshopping";
constexpr char* c_strActiveListOrderingGoShopping	= "actives/listordering_goshopping";

constexpr char* c_strLastExportFolder				= "folders/lastexportfolder";
constexpr char* c_strLastImportFolder				= "folders/lastimportfolder";

constexpr char* c_strLastShoppingListExportFolder	= "folders/lastshoppinglistexportfolder";
constexpr char* c_strLastRecipeBookConfigurationExportFolder = "folders/lastrecipebookconfigexportfolder";

// On windows this should save to HKEY_CURRENT_USER\Software\phwidmer.ch\RecipeBook

RecipeBookSettings::RecipeBookSettings()
{
	QCoreApplication::setOrganizationName("phwidmer.ch");
	QCoreApplication::setOrganizationDomain("phwidmer.ch");
	QCoreApplication::setApplicationName("RecipeBook");

	m_AppRBFilePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
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

QString RecipeBookSettings::applicationRecipeBookSaveFile() const
{
	return m_AppRBFilePath + c_strAppRBSaveFilename;
}

QString RecipeBookSettings::applicationRecipeBookAppsDataFolder() const
{
	return m_AppRBFilePath;
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

QString RecipeBookSettings::lastUsedExportFolder() const
{
	QSettings settings;
	QString defaultValue = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
	return settings.value(c_strLastExportFolder, defaultValue).toString();
}

void RecipeBookSettings::setLastUsedExportFolder(QString strFolder)
{
	QSettings settings;
	settings.setValue(c_strLastExportFolder, strFolder);
}

QString RecipeBookSettings::lastUsedImportFolder() const
{
	QSettings settings;
	QString defaultValue = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
	return settings.value(c_strLastImportFolder, defaultValue).toString();
}

void RecipeBookSettings::setLastUsedImportFolder(QString strFolder)
{
	QSettings settings;
	settings.setValue(c_strLastImportFolder, strFolder);
}

QString RecipeBookSettings::lastUsedShoppingListExportFolder() const
{
	QSettings settings;
	QString defaultValue = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
	return settings.value(c_strLastShoppingListExportFolder, defaultValue).toString();
}

void RecipeBookSettings::setLastUsedShoppingListExportFolder(QString strFolder) const
{
	QSettings settings;
	settings.setValue(c_strLastShoppingListExportFolder, strFolder);
}

QString RecipeBookSettings::lastUsedRecipeBookConfigurationExportFolder() const
{
	QSettings settings;
	QString defaultValue = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
	return settings.value(c_strLastRecipeBookConfigurationExportFolder, defaultValue).toString();
}

void RecipeBookSettings::setLastUsedRecipeBookConfigurationExportFolder(QString strFolder) const
{
	QSettings settings;
	settings.setValue(c_strLastRecipeBookConfigurationExportFolder, strFolder);
}

QString RecipeBookSettings::getActiveSortOrderGoShopping() const
{
	QSettings settings;
	return settings.value(c_strActiveSortOrderGoShopping, "").toString();
}

void RecipeBookSettings::setActiveSortOrderGoShopping(QString strSortOrder)
{
	QSettings settings;
	settings.setValue(c_strActiveSortOrderGoShopping, strSortOrder);
}

QString RecipeBookSettings::getActiveListOrderingGoShopping() const
{
	QSettings settings;
	return settings.value(c_strActiveListOrderingGoShopping, "").toString();
}

void RecipeBookSettings::setActiveListOrderingGoShopping(QString strSortOrder)
{
	QSettings settings;
	settings.setValue(c_strActiveListOrderingGoShopping, strSortOrder);
}
