#include "RBLatexExporter.h"
#include <QDir>
#include <QStandardPaths>
#include <QFile>
#include <QProcess>
#include "RBDialogInterface.h"

constexpr bool c_bRemoveTempFolder	= true;

constexpr char* c_texOutputFolder	= "/recipeBookTemp";
constexpr char* c_tempTexFilename	= "/tempShoppingList.tex";
constexpr char* c_tempPdfFilename	= "/tempShoppingList.pdf";

constexpr char* c_PdfLatexPath		= "E:\\programming\\RecipeBook\\miktex\\texmfs\\install\\miktex\\bin\\x64\\pdflatex.exe";

using namespace recipebook;

RBLatexExporter::RBLatexExporter()
{
}

bool RBLatexExporter::generatePdf(QString strLatexCode, QString strOutputFilename, const RBDialogInterface& rDlgInterface, quint32 uiCallCount)
{
	if(uiCallCount == 0)
	{
		return true;
	}

	if(!QFile::exists(c_PdfLatexPath))
	{
		rDlgInterface.showMessageBox(tr("Pdf generation failed"), tr("Couldn't find latex pdf generator."), RBDialogInterface::DlgType::Error);
		cleanUp();
		return false;
	}

	// Setup paths

	QString tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + c_texOutputFolder;
	if(!QDir(tempPath).mkpath("."))
	{
		rDlgInterface.showMessageBox(tr("Pdf generation failed"), tr("Couldn't create folder for temporary files"), RBDialogInterface::DlgType::Error);
		return false;
	}

	// Write tex file

	QFile texFile(tempPath + c_tempTexFilename);
	if (!texFile.open(QIODevice::WriteOnly))
	{
		rDlgInterface.showMessageBox(tr("Pdf generation failed"), tr("Couldn't create temporary file"), RBDialogInterface::DlgType::Error);
		cleanUp();
        return false;
    }
	texFile.write(strLatexCode.toUtf8());
	texFile.close();

	// Execute pdflatex and clean up
	
	QStringList arguments;
    arguments << tempPath + c_tempTexFilename;

	QProcess *myProcess = new QProcess(this);
	myProcess->setWorkingDirectory(tempPath);
	myProcess->connect(myProcess, 
					   QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
					   [&rDlgInterface, strLatexCode, uiCallCount, strOutputFilename, tempPath, this](int exitCode, QProcess::ExitStatus exitStatus)
	{
		Q_UNUSED(exitCode);
		Q_UNUSED(exitStatus);

		if(uiCallCount > 1)
		{
			generatePdf(strLatexCode, strOutputFilename, rDlgInterface, uiCallCount - 1);
			return;
		}

		if(QFile::exists(strOutputFilename))
		{
			QFile::remove(strOutputFilename);
		}
		if(QFile::copy(tempPath + c_tempPdfFilename, strOutputFilename))
		{
			rDlgInterface.showMessageBox(tr("Pdf generated"), tr("Pdf successfully exported to<br>\"%1\"").arg(strOutputFilename), RBDialogInterface::DlgType::Information);
		}
		else
		{
			rDlgInterface.showMessageBox(tr("Pdf generation failed"), tr("Error during pdf creation."), RBDialogInterface::DlgType::Error);
			qWarning("Couldn't copy shopping list pdf.");
		}

		cleanUp();
		deleteLater();
	});

	myProcess->start(c_PdfLatexPath, arguments);

	return true;
}

void RBLatexExporter::cleanUp()
{
	if(c_bRemoveTempFolder)
	{
		QString tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + c_texOutputFolder;
		QDir(tempPath).removeRecursively();
	}
}
