#ifndef RECIPEBOOK_TRANSLATION_MANAGER_H
#define RECIPEBOOK_TRANSLATION_MANAGER_H

#include <QTranslator>

#include <QQmlApplicationEngine>

namespace recipebook
{
	class RecipeBookSettings;
}

namespace recipebook::UI
{

	class RBTranslationManager : public QObject
	{
		Q_OBJECT

	public:
		RBTranslationManager(RecipeBookSettings& rSettings);

		void setQmlEngine(QQmlApplicationEngine& rEngine);

	public slots:
		QStringList availableLanguages() { return m_NamesOfAvailableLanguages; }
		QString getCurrentLanguageName() const;
		void setCurrentLanguageIndex(int index);

	private:
		void generateLanguagesList();
		void generateLanguageNames();

		QString getCurrentLanguage() const;
		void switchCurrentLanguage(QString lang);

	private:
		RecipeBookSettings& m_rSettings;
		QQmlApplicationEngine* m_pEngine;

		QStringList m_AvailableLanguages;
		QStringList m_NamesOfAvailableLanguages;

		QTranslator m_translator;
		QTranslator m_translatorQt;
	};
}

#endif
