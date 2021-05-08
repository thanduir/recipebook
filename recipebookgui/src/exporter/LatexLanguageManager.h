#ifndef LATEX_LANGUAGE_MANAGER_H
#define LATEX_LANGUAGE_MANAGER_H

#include <QTranslator>

#include <QQmlApplicationEngine>

namespace recipebook
{
	class RecipeBookSettings;
}

namespace recipebook
{

	class LatexLanguageManager : public QObject
	{
		Q_OBJECT

	public:
		LatexLanguageManager();

		QString getBabelStringFromLangCode(QString code) const;

	public slots:
		QStringList availableLanguages() const;
		QString getLanguageCode(QString langName) const;
        qsizetype getIndexFromLanguageCode(QString langCode) const;

	private:
		QStringList m_LanguageNames;
		QStringList m_LanguageCodes;
		QStringList m_LanguageLatexNames;
	};
}

#endif
