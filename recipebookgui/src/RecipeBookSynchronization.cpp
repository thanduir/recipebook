#include "RecipeBookSynchronization.h"
#include <QDesktopServices>
#include <QFile>
#include "RecipeBookSettings.h"
#include "RBDialogInterface.h"

using namespace recipebook;
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
	// TODO: identify relevant file on server:
	//		- read relevant file id from settings if present
	//		- otherwise ask user if he wants to give id (copied from another program) or generate a new (unique) one
	//			-> Need to test that a user provieded id actually exists!
	// m_rbDropbox.setFileId("TEST");

	QFile fileSyncBase(m_rSettings.applicationRecipeBookSyncBaseFile());
	if(!fileSyncBase.exists())
	{
		// TODO: No three way sync possible as base file not present! -> What to do? Ask user which to keep! (merging would mean to keep everything / ask for everything...)
	}

	// TODO: dl relevant file, then load this, current and base file into (new!) RecipeBook instances
	// QByteArray strFile;
	// m_rbDropbox.getCurrentFileContent(strFile);
	
	// TODO: Save current state as syncbackup

	// TODO: merge files
	//		-> look for differences in the three files (-> make list for one data type (categories, ingredients etc.), look for renames etc., 
	//			then ask user about those that can't be handled automatically!
	//		-> merge in all 3 files after a change is accepted

	// TODO: upload file
	//		-> upload merged version and save new sync base as well as new "current" state!
	// QByteArray strFile;
	// m_rbDropbox.updateFileContent(strFile);

	// TODO: inform user that merging has completed successfully
}