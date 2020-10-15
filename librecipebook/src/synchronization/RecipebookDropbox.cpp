#include "synchronization/RecipebookDropbox.h"
#include <QString>
#include <QUrlQuery>
#include <QDesktopServices>
#include <QEventLoop>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

const QString c_strAppKey		= "wqqw42itb20sded";
const QString c_strAppSecret	= "kx6jshoshbyh0mt";

const int c_MaxSingleUpload = (150 * 1024 * 1024);
const qint32 c_Dropbox_Http_ErrorCode = 409;

const QString c_ApiUrl = "https://api.dropboxapi.com";
const QString c_ContentUrl = "https://content.dropboxapi.com";
const QString c_NotifyUrl = "https://notify.dropboxapi.com";

using namespace recipebook::synchronization;

RecipebookDropbox::RecipebookDropbox()
:	m_FileId("")
{
}

QUrl RecipebookDropbox::getAuthorizeUri(QString redirect_uri)
{
	QUrl url;
	url.setScheme("https");
	url.setHost("www.dropbox.com");
	url.setPath("/oauth2/authorize");

	QUrlQuery query;
	query.addQueryItem("response_type", "code");
	query.addQueryItem("client_id", c_strAppKey);
	if(!redirect_uri.isEmpty())
	{
		query.addQueryItem("redirect_uri", redirect_uri);
	}
	url.setQuery(query);

	return url;
}

bool RecipebookDropbox::generateAcessTokenFromCode(QString strAccessCode)
{
	QUrl url;
	url.setUrl(c_ApiUrl);
	url.setPath("/oauth2/token");

	QUrlQuery q;
	q.addQueryItem("grant_type", "authorization_code");
	q.addQueryItem("code", strAccessCode.toStdString().c_str());
	q.addQueryItem("client_id", c_strAppKey.toStdString().c_str());
	q.addQueryItem("client_secret", c_strAppSecret.toStdString().c_str());
	url.setQuery(q);

	QNetworkRequest req(url);
	req.setRawHeader("Content-Type", "application/x-www-form-urlencoded; charset=utf-8");

	QEventLoop loop;	
	QString errorInfo;
	QString strAccessToken;	

	QNetworkAccessManager mgr;
	QByteArray nothing2post;
	QNetworkReply* reply = mgr.post(req, nothing2post);

	QObject::connect(reply, &QNetworkReply::finished, [&]()
	{
		int error_code = reply->error();
		switch(error_code)
		{
		case 0:
			{
				QByteArray data = reply->readAll();
				if(!data.isEmpty())
				{
					QJsonDocument doc = QJsonDocument::fromJson(data);
					QJsonObject js_in = doc.object();
					strAccessToken = js_in["access_token"].toString();
				}
				break;
			}

		default:
			{
				QByteArray data = reply->readAll();
				errorInfo = QString("ERROR. Unexpected status %1").arg(error_code);
				errorInfo += data;
				errorInfo += "\n";
				errorInfo += reply->errorString();
				break;
			}
		}

		reply->deleteLater();
		loop.exit();
	});

	loop.exec();

	if(!errorInfo.isEmpty())
	{
		qWarning() << errorInfo;
		m_LastError = errorInfo;
		return false;
	}

	m_AccessToken = strAccessToken;
	return true;
}

bool RecipebookDropbox::setAccessToken(QString strAccessToken)
{
	if(strAccessToken.isEmpty())
	{
		return false;
	}

	m_AccessToken = strAccessToken;

	// Verify connection and access token

	QUrl url;
	url.setUrl(c_ApiUrl);
	url.setPath("/2/check/user");

	QNetworkRequest req(url);
	QString bearer = QString("Bearer %1").arg(m_AccessToken);
	req.setRawHeader("Authorization", bearer.toUtf8());	
	req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	QString json = QString("{\"query\": \"recipebook-TEST\"}");
	QByteArray postdata = json.toUtf8();

	QEventLoop loop;
	QString errorInfo;

	QNetworkAccessManager mgr;
	QNetworkReply* reply = mgr.post(req, postdata);

	QObject::connect(reply, &QNetworkReply::finished, [&]()
	{
		int error_code = reply->error();
		switch(error_code)
		{
		case 0:
		{
			QByteArray data = reply->readAll();
			if(data.isEmpty())
			{
				errorInfo = "Verification failed: no data received!";
			}
			else
			{
				QJsonDocument doc = QJsonDocument::fromJson(data);
				QJsonObject js_in = doc.object();
				if(js_in["result"].toString() != "recipebook-TEST")
				{
					errorInfo = "Verification failed: invalid data!";
				}
			}
			break;
		}

		default:
		{
			QByteArray data = reply->readAll();
			errorInfo = QString("ERROR. Unexpected status %1").arg(error_code);
			errorInfo += data;
			errorInfo += "\n";
			errorInfo += reply->errorString();
			break;
		}
		}

		reply->deleteLater();
		loop.exit();
	});

	loop.exec();

	if(!errorInfo.isEmpty())
	{
		qWarning() << errorInfo;
		m_LastError = errorInfo;
		return false;
	}

	return true;
}

void RecipebookDropbox::setFileId(QString fileId)
{
	m_FileId = fileId;

	m_DropboxFileRev.clear();
	m_DropboxFileId.clear();
}

RecipebookDropbox::DownloadStatus RecipebookDropbox::getCurrentFileContent(QByteArray& fileContent)
{
	if(m_AccessToken.isEmpty())
	{
		return DownloadStatus::InvalidToken;
	}
	if(m_FileId.isEmpty())
	{
		return DownloadStatus::InvalidFileId;
	}

	fileContent.clear();
	m_DropboxFileId.clear();
	m_DropboxFileRev.clear();

	QString filename = "/" + m_FileId + ".json";

	QUrl url;
	url.setUrl(c_ContentUrl);
	url.setPath("/2/files/download");

	QNetworkRequest req(url);
	QString bearer = QString("Bearer %1").arg(m_AccessToken);
	req.setRawHeader("Authorization", bearer.toUtf8());
	req.setRawHeader("Dropbox-API-arg", QString("{ \"path\": \"%1\" }").arg(filename).toUtf8());

	QEventLoop loop;
	QString errorInfo;

	QNetworkAccessManager mgr;
	QByteArray nothing2post;
	QNetworkReply* reply = mgr.post(req, nothing2post);

	DownloadStatus status = DownloadStatus::ConnectionError;
	QObject::connect(reply, &QNetworkReply::finished, [&]()
	{
		int error_code = reply->error();
		QByteArray data = reply->readAll();
		switch(error_code)
		{
			case 0:
			{
				fileContent = data;

				QByteArray header = reply->rawHeader("Dropbox-API-Result");
				QJsonDocument doc = QJsonDocument::fromJson(header);
				QJsonObject js_in = doc.object();				
				m_DropboxFileId = js_in["id"].toString();
				m_DropboxFileRev = js_in["rev"].toString();

				status = DownloadStatus::Success;
				break;
			}

			case 206:
			{
				if(data.contains("path/not_found"))
				{
					errorInfo = QString("ERROR. Unexpected status %1").arg(error_code);
					errorInfo += data;
					status = DownloadStatus::FileNotFound;
					break;
				}

				// Fall-through for other errors
			}

			default:
			{
				errorInfo = QString("ERROR. Unexpected status %1").arg(error_code);
				errorInfo += data;
				status = DownloadStatus::ConnectionError;
				break;
			}
		}

		reply->deleteLater();
		loop.exit();
	});

	loop.exec();

	if(!errorInfo.isEmpty())
	{
		qWarning() << errorInfo;
		m_LastError = errorInfo;
	}

	return status;
}

RecipebookDropbox::UploadStatus RecipebookDropbox::updateFileContent(QByteArray fileContent)
{
	if(m_AccessToken.isEmpty())
	{
		return UploadStatus::InvalidToken;
	}
	if(m_FileId.isEmpty())
	{
		return UploadStatus::InvalidFileId;
	}

	QString filename = "/" + m_FileId + ".json";

	QUrl url;
	url.setUrl(c_ContentUrl);
	url.setPath("/2/files/upload");

	QNetworkRequest req(url);
	QString bearer = QString("Bearer %1").arg(m_AccessToken);
	req.setRawHeader("Authorization", bearer.toUtf8());

	req.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
	QString json;
	if(!m_DropboxFileRev.isEmpty())
	{
		json = QString("{ \"path\": \"%1\", \"mode\": { \".tag\": \"update\", \"update\": \"%2\" }, \"strict_conflict\": true }")
			.arg(filename)
			.arg(m_DropboxFileRev);
	}
	else
	{
		json = QString("{ \"path\": \"%1\", \"mode\": \"add\" }")
			.arg(filename);
	}
	req.setRawHeader("Dropbox-API-arg", json.toUtf8());

	QEventLoop loop;
	QString errorInfo;

	QNetworkAccessManager mgr;
	QNetworkReply* reply = mgr.post(req, fileContent);

	UploadStatus status = UploadStatus::ConnectionError;
	QObject::connect(reply, &QNetworkReply::finished, [&]()
	{
		int error_code = reply->error();
		QByteArray data = reply->readAll();
		switch(error_code)
		{
			case 0:
			{
				QJsonDocument doc = QJsonDocument::fromJson(data);
				QJsonObject js_in = doc.object();
				m_DropboxFileRev = js_in["rev"].toString();
				status = UploadStatus::Success;
				break;
			}

			case 206:
			{
				if(data.contains("path/conflict/file"))
				{
					errorInfo = QString("ERROR. Unexpected status %1").arg(error_code);
					errorInfo += data;
					status = UploadStatus::FileChangedOnServer;
					break;
				}

				// Fall-through for other errors
			}

			default:
			{
				errorInfo = QString("ERROR. Unexpected status %1").arg(error_code);
				errorInfo += reply->readAll();
				status = UploadStatus::ConnectionError;
				break;
			}
		}

		reply->deleteLater();
		loop.exit();
	});

	loop.exec();

	if(!errorInfo.isEmpty())
	{
		qWarning() << errorInfo;
		m_LastError = errorInfo;
	}

	return status;
}
