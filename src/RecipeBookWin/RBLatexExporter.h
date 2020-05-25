#ifndef RECIPEBOOK_LATEX_EXPORTER_H
#define RECIPEBOOK_LATEX_EXPORTER_H

#include <QObject>
#include <QString>

class UIStringConverter;

namespace recipebook
{
	class GoShoppingListItem;
	class SortedShoppingList;
	class RBDialogInterface;

	class RBLatexExporter : public QObject
	{
		Q_OBJECT

	public:
		RBLatexExporter();

		bool generatePdf(QString strLatexCode, QString strOutputFilename, const RBDialogInterface& rDlgInterface);

	private:
		void cleanUp();
	};
}

#endif
