#include "RecipeBookExporter.h"
#include <QUrl>
#include <QFileInfo>
#include <data/RBDataHandler.h>
#include "RecipeBookSettings.h"
#include "UIStringConverter.h"

using namespace recipebook;

RecipeBookExporter::RecipeBookExporter(RBDataHandler& rRBDataHandler,
									   RecipeBookSettings& rSettings, 
									   const UIStringConverter& rConverter,
									   const RBDialogInterface& rDlgInterface)
:	m_rRBDataHandler(rRBDataHandler), 
	m_rSettings(rSettings), 
	m_rConverter(rConverter),
	m_rDlgInterface(rDlgInterface)
{
}

QStringList RecipeBookExporter::getDlgNameFilters() const
{
	QStringList list;
	list.append(tr("pdf files (*.pdf)"));
	return list;
}

void RecipeBookExporter::exportRecipeBook(QString strFileURL, quint32 uiConfiguration)
{
	QString localFileName = QUrl(strFileURL).toLocalFile();

	QFileInfo fi(localFileName);
	m_rSettings.setLastUsedRecipeBookConfigurationExportFolder(fi.absolutePath());

	// TODO: Start latex generator with the correct data!
	uiConfiguration;
	/*// Read list

	SortedShoppingList list;
	{
		recipebook::RBDataWriteHandle handle(m_rRBDataHandler);
		const RecipeBook& rRecipeBook = handle.data();

		if(!rRecipeBook.existsSortOrder(strSortOrder))
		{
			return;
		}

		list.updateList(handle.data());
		list.changeSortOrder(handle.data().getSortOrder(strSortOrder), SortedShoppingListOrdering::Combined);
	}

	// Generate pdf

	switch(m_ExporterType)
	{
		case ExporterType::QtHtml:
		{
			ShoppingListExporterQtHtml exporter(m_rConverter);
			exporter.exportShoppingList(localFileName, list);
			break;
		}

		case ExporterType::Latex:
		{
			ShoppingListExporterLatex exporter(m_rConverter);
			exporter.exportShoppingList(localFileName, list, m_rDlgInterface);
			break;
		}
	}*/
}
