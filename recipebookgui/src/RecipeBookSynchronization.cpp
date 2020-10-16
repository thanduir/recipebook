#include "RecipeBookSynchronization.h"
#include <QDesktopServices>
#include <QFile>
#include <data/RecipeBook.h>
#include <data/RBDataHandler.h>
#include "serialization/RecipeBookSerializerFactory.h"
#include "RecipeBookSettings.h"
#include "RBDialogInterface.h"

using namespace recipebook;
using namespace recipebook::serialization;
using rbStatus = synchronization::RecipebookDropbox::Status;

RecipeBookSynchronization::RecipeBookSynchronization(RBDataHandler& rRBDataHandler,
													 RecipeBookSettings& rSettings,
													 const RBDialogInterface& rDlgInterface)
:	m_rRBDataHandler(rRBDataHandler),
	m_rSettings(rSettings),
	m_rDlgInterface(rDlgInterface)
{
}

QObject* RecipeBookSynchronization::getDlgObject() const
{
	if(!m_pEngine)
	{
		return nullptr;
	}

	QObject* rootObject = m_pEngine->rootObjects().first();
	if(!rootObject)
	{
		return nullptr;
	}

	return rootObject->findChild<QObject*>(QStringLiteral("appSyncUI"));
}

void RecipeBookSynchronization::execute()
{
	QString strAccessToken = m_rSettings.getSyncAccessToken();
	if(!strAccessToken.isEmpty())
	{
		rbStatus status = m_rbDropbox.setAccessToken(strAccessToken);
		if(status == rbStatus::Success)
		{
			// Token successfully set
			performMerge();
			return;
		}
		else if(status == rbStatus::InvalidTokenOrCode)
		{
			// Invalid token. Reset and fall-through to get a new one
			m_rSettings.setSyncAccessToken("");
		}
		else
		{
			// Connection problem
			m_rDlgInterface.showMessageBox(tr("Connection problem"), tr("Please check your internet connection and try again."), RBDialogInterface::DlgType::Error);
			return;
		}
	}	

	// No access token present yet
	QUrl url = m_rbDropbox.getAuthorizeUri();
	QDesktopServices::openUrl(url);

	// Ask user for code
	QObject* dlgObject = getDlgObject();
	if(dlgObject == nullptr)
	{
		return;
	}
	QMetaObject::invokeMethod(dlgObject, "askForAccessCode");
}

void RecipeBookSynchronization::setAccessCode(QString accessCode)
{
	// Generate and set access code
	rbStatus status = m_rbDropbox.generateAcessTokenFromCode(accessCode);
	if(status == rbStatus::Success)
	{
		// Save access token for later reuse
		m_rSettings.setSyncAccessToken(m_rbDropbox.getAccessToken());

		performMerge();
	}
	else if(status == rbStatus::InvalidTokenOrCode)
	{
		m_rDlgInterface.showMessageBox(tr("Invalid code"), tr("Code not accepted. Please try again."), RBDialogInterface::DlgType::Error);
	}
	else
	{
		m_rDlgInterface.showMessageBox(tr("Connection problem"), tr("Please check your internet connection and try again."), RBDialogInterface::DlgType::Error);
	}
}

void RecipeBookSynchronization::performMerge()
{
	QSharedPointer<IRBReader> spReader = SerializerFactory::getReader(FileFormat::Json);
	QSharedPointer<IRBWriter> spWriter = SerializerFactory::getWriter(FileFormat::Json, m_rSettings.getApplicationInstanceUID());

	// Read file from server

	QString strDropfileId = m_rSettings.getSyncFileId();
	m_rbDropbox.setFileId(strDropfileId);

	QByteArray dataServerFile;
	rbStatus status = m_rbDropbox.getCurrentFileContent(dataServerFile);
	if(status == rbStatus::InvalidFileId)
	{
		// No valid file id
		m_rSettings.setSyncFileId("");

		// TODO: No valid id saved -> ask user if he wants to give id (copied from another program) or generate a new (unique) one
		//			-> Need to test that a user provieded id actually exists!
	}
	else if(status != rbStatus::Success)
	{
		m_rDlgInterface.showMessageBox(tr("Connection problem"), tr("Please check your internet connection and try again."), RBDialogInterface::DlgType::Error);
		return;
	}

	RecipeBook recipeBookServer;
	RBMetaData metaDataServer;
	spReader->serialize(dataServerFile, metaDataServer, recipeBookServer);

	// Backups of server file
	QFile fileSyncBackupServer(m_rSettings.applicationRecipeBookSyncBackupServerFile());
	if(!spWriter->serialize(recipeBookServer, fileSyncBackupServer))
	{
		m_rDlgInterface.showMessageBox(tr("File write problem"),
									   tr("Couldn't write sync backup server file. Please check write access and free space for app data folder."),
									   RBDialogInterface::DlgType::Error);
		return;
	}

	// Read current file

	// Backups of current file
	QFile fileSyncBackupCurrent(m_rSettings.applicationRecipeBookSyncBackupCurrentFile());
	{
		recipebook::RBDataReadHandle handle(m_rRBDataHandler);
		if(!spWriter->serialize(handle.data(), fileSyncBackupCurrent))
		{
			m_rDlgInterface.showMessageBox(tr("File write problem"), 
										   tr("Couldn't write sync backup current file. Please check write access and free space for app data folder."), 
										   RBDialogInterface::DlgType::Error);
			return;
		}
	}
	RecipeBook recipeBookCurrent;
	RBMetaData metaDataCurrent;
	spReader->serialize(fileSyncBackupCurrent, metaDataCurrent, recipeBookCurrent);

	// Read base file

	QFile fileSyncBase(m_rSettings.applicationRecipeBookSyncBaseFile());
	if(!fileSyncBase.exists())
	{
		// TODO: No three way sync possible as base file not present! -> What to do? Just ask user which to keep!
		return;
	}
	RecipeBook recipeBookBase;
	RBMetaData metaDataBase;
	spReader->serialize(fileSyncBase, metaDataBase, recipeBookBase);

	// Backups of base file
	QFile fileSyncBackupBase(m_rSettings.applicationRecipeBookSyncBackupBaseFile());
	if(!spWriter->serialize(recipeBookBase, fileSyncBackupBase))
	{
		m_rDlgInterface.showMessageBox(tr("File write problem"),
									   tr("Couldn't write sync backup base file. Please check write access and free space for app data folder."),
									   RBDialogInterface::DlgType::Error);
		return;
	}

	// TODO: merge files
	//		-> look for differences in the three files (-> make list for one data type (categories, ingredients etc.), look for renames etc., 
	//			then ask user about those that can't be handled automatically!
	//		-> merge in all 3 files after a change is accepted
	//		-> HOW TO SAVE DATA AND CURRENT STATE BETWEEN QUESTIONS FOR USER? AND HOW TO SEND THIS DATA TO THE USER?

	// TODO: Show all / summary of changes to user and ask for confirmation! (how to do that? keeping track of changes or use copy of original data?)

	// TODO: upload file
	//		-> upload merged version and save new sync base as well as new "current" state!
	// QByteArray strFile;
	// m_rbDropbox.updateFileContent(strFile);

	// TODO: inform user that merging has completed successfully
}