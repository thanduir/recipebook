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
	if(!readServerFile())
	{
		cleanUp();
		return;
	}

	if(!readCurrentFile())
	{
		cleanUp();
		return;
	}

	if(!readBaseFile())
	{
		cleanUp();
		return;
	}

	bool bThreeWayMerge = m_spRBBase != nullptr;

	bThreeWayMerge;
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

	cleanUp();
}

bool RecipeBookSynchronization::readServerFile()
{
	// Read file from server

	QString strDropfileId = m_rSettings.getSyncFileId();
	m_rbDropbox.setFileId(strDropfileId);

	QByteArray dataServerFile;
	rbStatus status = m_rbDropbox.getCurrentFileContent(dataServerFile);
	if(status == rbStatus::InvalidFileId)
	{
		// Invalid (or unset) file id
		m_rSettings.setSyncFileId("");

		m_rDlgInterface.showMessageBox(tr("No or invalid serer file id set"),
									   tr("Please set correct file id or generate a new one in settings."),
									   RBDialogInterface::DlgType::Error);
		return false;
	}
	else if(status != rbStatus::Success)
	{
		m_rDlgInterface.showMessageBox(tr("Connection problem"), tr("Please check your internet connection and try again."), RBDialogInterface::DlgType::Error);
		return false;
	}

	m_spRBServer = QSharedPointer<RecipeBook>::create();

	RBMetaData metaDataServer;
	QSharedPointer<IRBReader> spReader = SerializerFactory::getReader(FileFormat::Json);
	spReader->serialize(dataServerFile, metaDataServer, *m_spRBServer);

	// Backup of server file

	QSharedPointer<IRBWriter> spWriter = SerializerFactory::getWriter(FileFormat::Json, m_rSettings.getApplicationInstanceUID());
	QFile fileSyncBackupServer(m_rSettings.applicationRecipeBookSyncBackupServerFile());
	if(!spWriter->serialize(*m_spRBServer, fileSyncBackupServer))
	{
		m_rDlgInterface.showMessageBox(tr("File write problem"),
									   tr("Couldn't write sync backup server file. Please check write access and free space for app data folder."),
									   RBDialogInterface::DlgType::Error);
		return false;
	}

	return true;
}

bool RecipeBookSynchronization::readCurrentFile()
{
	// Backup of current file
	QFile fileSyncBackupCurrent(m_rSettings.applicationRecipeBookSyncBackupCurrentFile());
	{
		QSharedPointer<IRBWriter> spWriter = SerializerFactory::getWriter(FileFormat::Json, m_rSettings.getApplicationInstanceUID());

		recipebook::RBDataReadHandle handle(m_rRBDataHandler);
		if(!spWriter->serialize(handle.data(), fileSyncBackupCurrent))
		{
			m_rDlgInterface.showMessageBox(tr("File write problem"),
										   tr("Couldn't write sync backup current file. Please check write access and free space for app data folder."),
										   RBDialogInterface::DlgType::Error);
			return false;
		}
	}
	
	// Read current file from backup

	m_spRBCurrent = QSharedPointer<RecipeBook>::create();

	RBMetaData metaDataCurrent;
	QSharedPointer<IRBReader> spReader = SerializerFactory::getReader(FileFormat::Json);
	spReader->serialize(fileSyncBackupCurrent, metaDataCurrent, *m_spRBCurrent);

	return true;
}

bool RecipeBookSynchronization::readBaseFile()
{
	// Read base file

	QFile fileSyncBase(m_rSettings.applicationRecipeBookSyncBaseFile());
	if(!fileSyncBase.exists())
	{
		// No base file present. This is not an error, but no three way merge will be possible
		m_spRBBase = nullptr;
		return true;
	}
	
	m_spRBBase = QSharedPointer<RecipeBook>::create();

	RBMetaData metaDataBase;
	QSharedPointer<IRBReader> spReader = SerializerFactory::getReader(FileFormat::Json);
	spReader->serialize(fileSyncBase, metaDataBase, *m_spRBBase);

	// Backups of base file

	QFile fileSyncBackupBase(m_rSettings.applicationRecipeBookSyncBackupBaseFile());
	QSharedPointer<IRBWriter> spWriter = SerializerFactory::getWriter(FileFormat::Json, m_rSettings.getApplicationInstanceUID());
	if(!spWriter->serialize(*m_spRBBase, fileSyncBackupBase))
	{
		m_rDlgInterface.showMessageBox(tr("File write problem"),
									   tr("Couldn't write sync backup base file. Please check write access and free space for app data folder."),
									   RBDialogInterface::DlgType::Error);
		return false;
	}

	return true;
}

void RecipeBookSynchronization::cleanUp()
{
	m_spRBServer = nullptr;
	m_spRBBase = nullptr;
	m_spRBCurrent = nullptr;
}
