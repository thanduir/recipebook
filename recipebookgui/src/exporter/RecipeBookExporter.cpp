#include "RecipeBookExporter.h"
#include <QUrl>
#include <QFileInfo>
#include <QException>
#include <QThread>
#include <data/RBDataHandler.h>
#include "../RecipeBookSettings.h"
#include "../uistringconverter.h"
#include "../RBDialogInterface.h"
#include "RecipeBookExporterPodofo.h"

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

	QThread* thread = QThread::create([this, uiConfiguration, localFileName]
	{
		m_rDlgInterface.lockUI();

		bool bSuccess = false;
		RecipeBookExporterPodofo exporter(m_rConverter);
		{
			recipebook::RBDataReadHandle handle(m_rRBDataHandler);
			const RecipeBook& rRecipeBook = handle.data();

			if(uiConfiguration >= rRecipeBook.getConfigurationsCount())
			{
				throw QException();
			}

			bSuccess = exporter.writeDocument(rRecipeBook.getConfigurationAt(uiConfiguration), localFileName);
		}

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
}
