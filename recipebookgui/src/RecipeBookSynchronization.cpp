#include "RecipeBookSynchronization.h"
#include <QDesktopServices>
#include <QFile>
#include <QUuid>
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

void RecipeBookSynchronization::generateAndSetUniqueFileId()
{
	QString newID = QUuid::createUuid().toString(QUuid::WithoutBraces);

	QFile fileSyncBase(m_rSettings.applicationRecipeBookSyncBaseFile());
	if(fileSyncBase.exists())
	{
		fileSyncBase.remove();
	}

	m_rSettings.setSyncFileId(newID);
}

void RecipeBookSynchronization::setServerFileId(QString strId)
{
	QFile fileSyncBase(m_rSettings.applicationRecipeBookSyncBaseFile());
	if(fileSyncBase.exists())
	{
		fileSyncBase.remove();
	}

	m_rSettings.setSyncFileId(strId);
}

void RecipeBookSynchronization::checkSyncReminder()
{
	quint32 uiInterval = m_rSettings.getSyncReminderInterval();
	if(uiInterval == 0)
	{
		return;
	}

	QDate dateLastChecked = m_rSettings.getDateLastSync();
	QDate dateNow = QDate::currentDate();

	if(dateLastChecked.isNull() || dateLastChecked.addDays(uiInterval) <= dateNow)
	{
		// Show reminder
		QObject* dlgObject = getDlgObject();
		if(dlgObject == nullptr)
		{
			return;
		}
		QMetaObject::invokeMethod(dlgObject, "askForSync");
	}
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

    bool bNoMergeBase = m_spRBBase != nullptr;
	bool bUploadOnly = m_spRBServer == nullptr;
	if(bUploadOnly)
	{
		if(!uploadFile(m_spRBCurrent))
		{
			cleanUp();
			return;
		}

		if(!updateBase(m_spRBCurrent))
		{
			cleanUp();
			return;
		}

		m_rSettings.setDateLastSyncNow();
		cleanUp();

		m_rDlgInterface.showMessageBox(tr("Synchronization successful"),
									   tr("Current state successfully synchronized with server (no changes to local data)."),
									   RBDialogInterface::DlgType::Information);
	}
	else if(!bNoMergeBase)
	{
		// No merge base exists -> Ask user which file to keep -> this then calls twoWayMerge
		QObject* dlgObject = getDlgObject();
		if(dlgObject == nullptr)
		{
			return;
		}
		QMetaObject::invokeMethod(dlgObject, "askWhichFileToKeep");
	}
	else
	{
		performThreeWayMerge();
	}
}

void RecipeBookSynchronization::performTwoWayMerge(bool bKeepServerSide)
{
	QString msg;
	if(bKeepServerSide)
	{
		if(!updateCurrent(m_spRBServer))
		{
			cleanUp();
			return;
		}

		if(!updateBase(m_spRBServer))
		{
			cleanUp();
			return;
		}

		msg = tr("Successfully updated current local data with the current server data.");
	}
	else
	{
		if(!uploadFile(m_spRBCurrent))
		{
			cleanUp();
			return;
		}

		if(!updateBase(m_spRBCurrent))
		{
			cleanUp();
			return;
		}

		msg = tr("Successfully updated current server data with the current local data.");
	}

	m_rSettings.setDateLastSyncNow();
	cleanUp();

	m_rDlgInterface.showMessageBox(tr("Synchronization successful"),
								   msg,
								   RBDialogInterface::DlgType::Information);
}

void RecipeBookSynchronization::performThreeWayMerge()
{
	// TODO: merge files
	//		-> look for differences in the three files (-> make list for one data type (categories, ingredients etc.), look for renames etc., 
	//			then ask user about those that can't be handled automatically!
	//		-> merge in all 3 files after a change is accepted
	//		-> HOW TO SAVE DATA AND CURRENT STATE BETWEEN QUESTIONS FOR USER? AND HOW TO SEND THIS DATA TO THE USER?

	// TODO: Show all / summary of changes to user and ask for confirmation! (how to do that? keeping track of changes or use copy of original data?)

	// TODO: inform user that merging has completed successfully
	// TODO: Call m_rSettings.setDateLastSyncNow() and cleanUp() whereever appropriate
}

bool RecipeBookSynchronization::readServerFile()
{
	// Read file from server

	QString strDropboxFileId = m_rSettings.getSyncFileId();
	if(strDropboxFileId.isEmpty())
	{
		m_rDlgInterface.showMessageBox(tr("No server file id set"),
									   tr("Please set correct file id or generate a new one in settings."),
									   RBDialogInterface::DlgType::Error);
		return false;
	}

	m_rbDropbox.setFileId(strDropboxFileId);

	QByteArray dataServerFile;
	rbStatus status = m_rbDropbox.getCurrentFileContent(dataServerFile);
	if(status == rbStatus::InvalidFileId)
	{
		// File not present yet
		m_spRBServer = nullptr;
		return true;
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

bool RecipeBookSynchronization::uploadFile(QSharedPointer<RecipeBook> spFile)
{
	QSharedPointer<IRBWriter> spWriter = SerializerFactory::getWriter(FileFormat::Json, m_rSettings.getApplicationInstanceUID());

	QByteArray dataFile;
	if(!spWriter->serialize(*spFile, dataFile))
	{
		// Generelles Problem, sollte eigentlich gar nicht auftreten.
		m_rDlgInterface.showMessageBox(tr("Unspecified problem"),
									   tr("Could not save current state to file."),
									   RBDialogInterface::DlgType::Error);
		return false;
	}
	
	rbStatus status = m_rbDropbox.updateFileContent(dataFile);
	if(status == rbStatus::InvalidFileId || status == rbStatus::FileChangedOnServer)
	{
		// Generelles Problem, sollte eigentlich gar nicht auftreten.
		m_rDlgInterface.showMessageBox(tr("Unspecified problem"),
									   tr("Invalid file on server."),
									   RBDialogInterface::DlgType::Error);
		return false;
	}
	else
	{
		// Connection problem
		m_rDlgInterface.showMessageBox(tr("Connection problem"), tr("Please check your internet connection and try again."), RBDialogInterface::DlgType::Error);
		return false;
	}

	return true;
}

bool RecipeBookSynchronization::updateBase(QSharedPointer<RecipeBook> spFile)
{
	QSharedPointer<IRBWriter> spWriter = SerializerFactory::getWriter(FileFormat::Json, m_rSettings.getApplicationInstanceUID());

	QFile fileSyncBase(m_rSettings.applicationRecipeBookSyncBaseFile());
	if(!spWriter->serialize(*spFile, fileSyncBase))
	{
		m_rDlgInterface.showMessageBox(tr("Unspecified problem"),
									   tr("Could not save state to sync base file."),
									   RBDialogInterface::DlgType::Error);
		return false;
	}

	return true;
}

bool RecipeBookSynchronization::updateCurrent(QSharedPointer<RecipeBook> spFile)
{
	QSharedPointer<IRBWriter> spWriter = SerializerFactory::getWriter(FileFormat::Json, m_rSettings.getApplicationInstanceUID());
	QByteArray data;
	if(!spWriter->serialize(*spFile, data))
	{
		m_rDlgInterface.showMessageBox(tr("File write problem"),
									   tr("Couldn't read updated data."),
									   RBDialogInterface::DlgType::Error);
		return false;
	}

	QSharedPointer<IRBReader> spReader = SerializerFactory::getReader(FileFormat::Json);

	recipebook::RBDataWriteHandle handle(m_rRBDataHandler);
	RBMetaData metaData;
	if(!spReader->serialize(data, metaData, handle.data()))
	{
		m_rDlgInterface.showMessageBox(tr("File write problem"),
										tr("Couldn't update current data."),
										RBDialogInterface::DlgType::Error);
		return false;
	}

	emit signalCurrentDataUpdated();

	return true;
}

void RecipeBookSynchronization::cleanUp()
{
	m_spRBServer = nullptr;
	m_spRBBase = nullptr;
	m_spRBCurrent = nullptr;
}
