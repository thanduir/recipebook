#ifndef RECIPEBOOK_CONFIGURATION_EXPORTER_LATEX_H
#define RECIPEBOOK_CONFIGURATION_EXPORTER_LATEX_H

#include <QObject>

class QTextStream;
class UIStringConverter;

namespace recipebook
{
	class RecipeBookConfiguration;
	class RecipeBookSettings;
	class Recipe;
	class RBDialogInterface;
	class LatexLanguageManager;

	class RecipeBookExporterLatex : public QObject
	{
		Q_OBJECT

	public:
		RecipeBookExporterLatex(const UIStringConverter& rConverter, const RecipeBookSettings& rSettings);

		void generateLatex(const RecipeBookConfiguration& rConfig, const LatexLanguageManager& rLanguageManager);
		void exportRecipeBook(QString strFilename, const RBDialogInterface& rDlgInterface);

	private:
		void addRecipe(QTextStream& rStream, quint32 uiCurrentLevel, const Recipe& rRecipe) const;
		QString formatItems(const Recipe& rRecipe) const;

	private:
		QString m_Latex;
		const UIStringConverter& m_rConverter;
		const RecipeBookSettings& m_rSettings;
	};
}

#endif
