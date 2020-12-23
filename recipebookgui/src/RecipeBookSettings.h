#ifndef RECIPEBOOK_SETTINGS_H
#define RECIPEBOOK_SETTINGS_H

#include <QSettings>
#include <data/Unit.h>

namespace recipebook
{
	class RecipeBookSettings : public QObject
	{
		Q_OBJECT

	public:
		RecipeBookSettings();

	public slots:
		QString applicationRecipeBookSaveFile() const;
		QString applicationRecipeBookAppsDataFolder() const;
		QString getApplicationInstanceUID();

		QString applicationUserFilesPathAndroid() const;
		QStringList getCurrentFilenamesAndroid() const;

		QString getCurrentAppLanguage() const;
		void setCurrentAppLanguage(QString lang);

		quint32 getDefaultUnit() const;
		void setDefaultUnit(quint32 defaultUnit);

		quint32 getDefaultRecipeNrPersons() const;
		void setDefaultRecipeNrPersons(quint32 nrPersons);

		QString getDefaultCategory() const;
		void setDefaultCategory(QString strCategory);

		QString getDefaultSortOrder() const;
		void setDefaultSortOrder(QString strSortOrder);

		QString lastUsedExportFolder() const;
		void setLastUsedExportFolder(QString strFolder);

		QString lastUsedImportFolder() const;
		void setLastUsedImportFolder(QString strFolder);

		QString lastUsedShoppingListExportFolder() const;
		void setLastUsedShoppingListExportFolder(QString strFolder) const;

		QString lastUsedRecipeBookConfigurationExportFolder() const;
		void setLastUsedRecipeBookConfigurationExportFolder(QString strFolder) const;

		QString getActiveSortOrderGoShopping() const;
		void setActiveSortOrderGoShopping(QString strSortOrder);

		QString getActiveListOrderingGoShopping() const;
		void setActiveListOrderingGoShopping(QString strSortOrder);

		bool pdfLatexExporterAvailable() const;
		QString getPdfLatexExeNameFilter() const;
		QString getPdfLatexFile() const;
		void setPdfLatexFile(QString filePath);

		QString getSyncAccessToken() const;
		void setSyncAccessToken(QString strToken);

		QString getSyncFileId() const;
		void setSyncFileId(QString strFileId);

		quint32 getSyncReminderInterval() const;
		void setSyncReminderInterval(quint32 uiInterval);

		QString applicationRecipeBookSyncBaseFile() const;
		QString applicationRecipeBookSyncBackupLocalFile() const;
		QString applicationRecipeBookSyncBackupServerFile() const;
		QString applicationRecipeBookSyncBackupBaseFile() const;

	public:
		QDate getDateLastSync() const;
		void setDateLastSyncNow();

	signals:
		void loadDefaultData() const;
		void resetAllData() const;

	private:
		QString m_AppRBFilePath;
		QString m_AndroidDataPath;
	};
}

#endif
