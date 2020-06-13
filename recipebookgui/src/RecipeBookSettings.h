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

	signals:
		void loadDefaultData() const;
		void resetAllData() const;

	private:
		QString m_AppRBFilePath;
	};
}

#endif
