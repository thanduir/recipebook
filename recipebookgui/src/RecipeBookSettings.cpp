#include "RecipeBookSettings.h"
#include <QCoreApplication>
#include <QSettings>
#include <QStandardPaths>
#include <QUuid>
#include <QDir>
#include <QUrl>

using namespace recipebook;

constexpr const char* c_strAppRBSaveFilename			= "/RecipeBook.json";
constexpr const char* c_strAppRBSyncBaseFilename		= "/RecipeBook.syncbase.json"; 
constexpr const char* c_strAppRBSyncBkupCurrentFilename	= "/rbCurrent.syncbkup.json";
constexpr const char* c_strAppRBSyncBkupBaseFilename	= "/rbBase.syncbkup.json";
constexpr const char* c_strAppRBSyncBkupServerFilename	= "/rbServer.syncbkup.json";

constexpr const char* c_strAppInstanceUID				= "appInstanceUID";

constexpr const char* c_strAppCurrentLanguage			= "currentLanguage";

constexpr const char* c_strDefaultUnit					= "defaults/unit";
constexpr const char* c_strDefaultNrPersons				= "defaults/nrpersons";
constexpr const char* c_strDefaultCategory				= "defaults/category";
constexpr const char* c_strDefaultSortOrder				= "defaults/sortorder";

constexpr const char* c_strActiveSortOrderGoShopping	= "actives/sortorder_goshopping";
constexpr const char* c_strActiveListOrderingGoShopping	= "actives/listordering_goshopping";

constexpr const char* c_strLastExportFolder				= "folders/lastexportfolder";
constexpr const char* c_strLastImportFolder				= "folders/lastimportfolder";

constexpr const char* c_strLastShoppingListExportFolder	= "folders/lastshoppinglistexportfolder";
constexpr const char* c_strLastRecipeBookConfigurationExportFolder = "folders/lastrecipebookconfigexportfolder";

constexpr const char* c_strPdfLatexPath					= "export/pdflatexpath";
constexpr const char* c_strPdfLatexExeNameFilter		= "pdflatex.exe";

constexpr const char* c_strSyncAccessToken				= "sync/accesstoken";
constexpr const char* c_strSyncFileId					= "sync/flieId";
constexpr const char* c_strSyncReminderInterval			= "sync/reminderInterval";

// On windows this should save to HKEY_CURRENT_USER\Software\phwidmer.ch\RecipeBook

RecipeBookSettings::RecipeBookSettings()
{
	QCoreApplication::setOrganizationName("phwidmer.ch");
	QCoreApplication::setOrganizationDomain("phwidmer.ch");
	QCoreApplication::setApplicationName("RecipeBook");

	m_AppRBFilePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	m_AndroidDataPath = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[1];
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

QString RecipeBookSettings::applicationUserFilesPathAndroid() const
{
	return m_AndroidDataPath;
}

QStringList RecipeBookSettings::getCurrentFilenamesAndroid() const
{
	QDir dir(m_AndroidDataPath);
	return dir.entryList(QStringList("*.json"));
}

QString RecipeBookSettings::getCurrentAppLanguage() const
{
	QSettings settings;
	if(!settings.contains(c_strAppCurrentLanguage))
	{
		return "";
	}

	return settings.value(c_strAppCurrentLanguage).toString();
}

void RecipeBookSettings::setCurrentAppLanguage(QString lang)
{
	QSettings settings;
	settings.setValue(c_strAppCurrentLanguage, lang);
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

bool RecipeBookSettings::pdfLatexExporterAvailable() const
{
	QString filePath = QUrl(getPdfLatexFile()).toLocalFile();
	return QFile::exists(filePath);
}

QString RecipeBookSettings::getPdfLatexExeNameFilter() const
{
	return c_strPdfLatexExeNameFilter;
}

QString RecipeBookSettings::getPdfLatexFile() const
{
	QSettings settings;
	return settings.value(c_strPdfLatexPath, "").toString();
}

void RecipeBookSettings::setPdfLatexFile(QString filePath)
{
	QSettings settings;
	settings.setValue(c_strPdfLatexPath, filePath);
}

QString RecipeBookSettings::getSyncAccessToken() const
{
	QSettings settings;
	return settings.value(c_strSyncAccessToken, "").toString();
}

void RecipeBookSettings::setSyncAccessToken(QString strToken)
{
	QSettings settings;
	settings.setValue(c_strSyncAccessToken, strToken);
}

QString RecipeBookSettings::getSyncFileId() const
{
	QSettings settings;
	return settings.value(c_strSyncFileId, "").toString();
}

void RecipeBookSettings::setSyncFileId(QString strFileId)
{
	QSettings settings;
	settings.setValue(c_strSyncFileId, strFileId);
}

quint32 RecipeBookSettings::getSyncReminderInterval() const
{
	QSettings settings;
	return settings.value(c_strSyncReminderInterval, 7).toUInt();
}

void RecipeBookSettings::setSyncReminderInterval(quint32 uiInterval)
{
	QSettings settings;
	settings.setValue(c_strSyncReminderInterval, uiInterval);
}

QString RecipeBookSettings::applicationRecipeBookSyncBaseFile() const
{
	return m_AppRBFilePath + c_strAppRBSyncBaseFilename;
}

QString RecipeBookSettings::applicationRecipeBookSyncBackupCurrentFile() const
{
	return m_AppRBFilePath + c_strAppRBSyncBkupCurrentFilename;
}

QString RecipeBookSettings::applicationRecipeBookSyncBackupServerFile() const
{
	return m_AppRBFilePath + c_strAppRBSyncBkupServerFilename;
}

QString RecipeBookSettings::applicationRecipeBookSyncBackupBaseFile() const
{
	return m_AppRBFilePath + c_strAppRBSyncBkupBaseFilename;
}
