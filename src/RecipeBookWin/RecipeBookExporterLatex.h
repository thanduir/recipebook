#ifndef RECIPEBOOK_CONFIGURATION_EXPORTER_LATEX_H
#define RECIPEBOOK_CONFIGURATION_EXPORTER_LATEX_H

#include <QObject>

class QTextStream;
class UIStringConverter;

namespace recipebook
{
	class RecipeBookConfiguration;
	class Recipe;
	class RecipeItem;
	class RBDialogInterface;

	class RecipeBookExporterLatex : public QObject
	{
		Q_OBJECT

	public:
		RecipeBookExporterLatex(const UIStringConverter& rConverter);

		void generateLatex(const RecipeBookConfiguration& rConfig);
		void exportRecipeBook(QString strFilename, const RBDialogInterface& rDlgInterface);

	private:
		void addRecipe(QTextStream& rStream, quint32 uiCurrentLevel, const Recipe& rItem) const;
		QString formatItem(const RecipeItem& rItem) const;

	private:
		QString m_Latex;
		const UIStringConverter& m_rConverter;
	};
}

#endif
