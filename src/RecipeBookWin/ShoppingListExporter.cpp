#include "ShoppingListExporter.h"
#include <QUrl>
#include <QFileInfo>
#include <data/SortedShoppingList.h>
#include <data/RBDataHandler.h>
#include "RecipeBookSettings.h"
#include "UIStringConverter.h"
#include "ShoppingListExporterQtHtml.h"
#include "ShoppingListExporterLatex.h"

using namespace recipebook;

ShoppingListExporter::ShoppingListExporter(RBDataHandler& rRBDataHandler, 
										   RecipeBookSettings& rSettings, 
										   const UIStringConverter& rConverter,
										   const RBDialogInterface& rDlgInterface)
:	m_rRBDataHandler(rRBDataHandler), 
	m_rSettings(rSettings), 
	m_rConverter(rConverter),
	m_rDlgInterface(rDlgInterface)
{
}

QStringList ShoppingListExporter::getDlgNameFilters() const
{
	QStringList list;
	list.append(tr("pdf files (*.pdf)"));
	return list;
}

void ShoppingListExporter::exportShoppingList(QString strFileURL, QString strSortOrder)
{
	QString localFileName = QUrl(strFileURL).toLocalFile();

	QFileInfo fi(localFileName);
	m_rSettings.setLastUsedShoppingListExportFolder(fi.absolutePath());

	// Read list

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
	}
}
