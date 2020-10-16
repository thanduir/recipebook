#ifndef RECIPEBOOK_SYNCHRONIZATION_RECIPEBOOK_DROPBOX_H
#define RECIPEBOOK_SYNCHRONIZATION_RECIPEBOOK_DROPBOX_H

#include <QtGlobal>
#include <QObject>
#include <QNetworkAccessManager>

namespace recipebook::synchronization
{
	/* 
		Reference: https://www.dropbox.com/developers/documentation/http/documentation
		Dropbox app console: https://www.dropbox.com/developers/apps
	*/
	class RecipebookDropbox final
	{
	public:
		enum class Status
		{
			Success,
			
			FileNotFound,
			FileChangedOnServer,

			ConnectionError,

			InvalidFileId,
			InvalidTokenOrCode
		};

	public:
		explicit RecipebookDropbox();

		/* Workflow:
			- No saved access token:
				1. open URL from getAuthorizeUri(..) so the user can authorize the app
				2. call generateAcessTokenFromCode with the code the user gets after authorization
				3. save accesstoken
			- Access token already saved:
				1. call setAccessToken with the saved token
		*/
		static QUrl getAuthorizeUri(QString redirect_uri = QString());

		Status generateAcessTokenFromCode(QString strAccessCode);
		Status setAccessToken(QString strAccessToken);
		QString getAccessToken() const { return m_AccessToken; }

		void setFileId(QString fileId);
		QString getFileId() const { return m_FileId; }

		Status getCurrentFileContent(QByteArray& fileContent);
		// The latest version of the file needs to be fetched and merged first, otherwise this may return FileChangedOnServer.
		Status updateFileContent(QByteArray fileContent);
		QString getCurrentDropboxFileRev() const { return m_DropboxFileRev; }
		QString getCurrentDropboxFileId() const { return m_DropboxFileId; }

		QString getLastError() const { return m_LastError; }

	private:
		QString m_AccessToken;
		QString m_LastError;

		QString m_FileId;
		QString m_DropboxFileRev;
		QString m_DropboxFileId;
	};
}

#endif
