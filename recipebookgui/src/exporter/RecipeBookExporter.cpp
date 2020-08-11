#include "RecipeBookExporter.h"
#include <QUrl>
#include <QFileInfo>
#include <QException>
#include <data/RBDataHandler.h>
#include "../RecipeBookSettings.h"
#include "../uistringconverter.h"
#include "RecipeBookExporterLatex.h"

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

	RecipeBookExporterLatex exporter(m_rConverter, m_rSettings);
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);
		const RecipeBook& rRecipeBook = handle.data();

		if(uiConfiguration >= rRecipeBook.getConfigurationsCount())
		{
			throw QException();
		}

		exporter.generateLatex(rRecipeBook.getConfigurationAt(uiConfiguration), m_Languages);
	}

	exporter.exportRecipeBook(localFileName, m_rDlgInterface);
}
