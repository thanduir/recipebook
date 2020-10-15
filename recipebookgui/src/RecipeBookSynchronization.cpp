#include "RecipeBookSynchronization.h"

using namespace recipebook;

RecipeBookSynchronization::RecipeBookSynchronization(RBDataHandler& rRBDataHandler,
													 RecipeBookSettings& rSettings)
:	m_rRBDataHandler(rRBDataHandler),
	m_rSettings(rSettings)
{
}

void RecipeBookSynchronization::execute()
{
	// TODO: Production code should:
	//			* access token:
	//				- load access token if already saved, then call setAccessToken on rbDropbox
	//				- otherwise get url and open webpage, then ask user for code, then generate and save access token
	//					* URL with getAuthorizeUri
	//					* then generateAcessTokenFromCode with code from user
	//					* then save token
	//			* identify relevant file:
	//				- read relevant file id from settings if present
	//				- otherwise ask user if he wants to give id (copied from another program) or generate a new (unique) one
	//					-> I need to test first that this id actually exists!
	//			* dl relevant file
	//				-> rbDropbox has saved the fileRev, therefore i need to keep the instance alive!
	//			* merge file
	//				-> save pre-sync state locally!
	//				-> handle third file for 3-way merge correctly (whether it's present or not!)!
	//			* upload file
	//				-> upload merged version and save merge base
	//		Other stuff:
	//			* settings UI for file id
	//				-> upon changing of file id, what should happen? (user says which to keep (server or local)? Or always take server version on change?)
	//				-> access token should be clearable / resetable!
	//				-> reset on server / replace with server version!

	/*QUrl url = rbDropbox.getAuthorizeUri();
	QDesktopServices::openUrl(url);
	// TODO: This will be supplied by the user...
	//QString strCode = "...";
	//QString strToken = rbDropbox.generateAcessTokenFromCode(strCode);*/

	// TODO: This access token should be handled differently
	//QString strToken = "cLNTxTPs6WEAAAAAAAAAAZPOAKQvBHPZHRdGg1hsup9ofBoeXIBgCU-k8Leil4rD";
	/*rbDropbox.setAccessToken(strToken);

	QByteArray strFile;
	rbDropbox.getCurrentFileContent(strFile);

	strFile = "CHANGED FILE5";
	rbDropbox.updateFileContent(strFile);*/
}
