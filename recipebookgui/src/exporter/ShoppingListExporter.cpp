#include "ShoppingListExporter.h"
#include <QUrl>
#include <QFileInfo>
#include <QThread>
#include <data/SortedShoppingList.h>
#include <data/RBDataHandler.h>
#include "../RecipeBookSettings.h"
#include "../uistringconverter.h"
#include "../RBDialogInterface.h"
#ifndef Q_OS_ANDROID
#include "ShoppingListExporterPodofo.h"
#endif

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
#ifdef Q_OS_ANDROID
    Q_UNUSED(strFileURL);
    Q_UNUSED(strSortOrder);
#else
	QString localFileName = QUrl(strFileURL).toLocalFile();

	QFileInfo fi(localFileName);
	m_rSettings.setLastUsedShoppingListExportFolder(fi.absolutePath());

	QThread* thread = QThread::create([this, strSortOrder, localFileName]
	{
		m_rDlgInterface.lockUI();

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

		ShoppingListExporterPodofo exporter(m_rConverter);
		bool bSuccess = exporter.writeDocument(localFileName, list);

		m_rDlgInterface.unlockUI();
		if(bSuccess)
		{
			m_rDlgInterface.showMessageBox(tr("Pdf generated"),
											tr("Pdf successfully exported to<br>\"%1\"").arg(localFileName),
											RBDialogInterface::DlgType::Information);
		}
		else
		{
			m_rDlgInterface.showMessageBox(tr("Pdf generation failed"),
											tr("Error during pdf creation. Is the file already opened?"),
											RBDialogInterface::DlgType::Error);
		}
	});
	thread->start();
#endif
}
