#ifndef RECIPEBOOK_EXPORTER_H
#define RECIPEBOOK_EXPORTER_H

#include <QObject>
#include "LatexLanguageManager.h"

class UIStringConverter;

namespace recipebook
{
	class RBDataHandler;
	class RecipeBookSettings;
	class RBDialogInterface;

	class RecipeBookExporter : public QObject
	{
		Q_OBJECT

	public:
		RecipeBookExporter(RBDataHandler& rRBDataHandler,
						   RecipeBookSettings& rSettings, 
						   const UIStringConverter& rConverter,
						   const RBDialogInterface& rDlgInterface);

	public slots:
		QStringList getDlgNameFilters() const;
		void exportRecipeBook(QString strFileURL, quint32 uiConfiguration);

		QStringList availableLanguages() const { return m_Languages.availableLanguages(); }
		QString getCodeFromLanguageName(QString langName) const { return m_Languages.getLanguageCode(langName); }
        qsizetype getIndexFromLanguageCode(QString langCode) const { return m_Languages.getIndexFromLanguageCode(langCode); }

	private:
		RBDataHandler&				m_rRBDataHandler;
		RecipeBookSettings&			m_rSettings;
		LatexLanguageManager		m_Languages;
		const UIStringConverter&	m_rConverter;
		const RBDialogInterface&	m_rDlgInterface;
	};
}

#endif
