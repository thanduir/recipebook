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

RecipebookDropbox::Status RecipebookDropbox::generateAcessTokenFromCode(QString strAccessCode)
{
	if(strAccessCode.isEmpty())
	{
		return Status::InvalidTokenOrCode;
	}

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

	Status status = Status::ConnectionError;
	QObject::connect(reply, &QNetworkReply::finished, [&]()
	{
		int error_code = reply->error();
		QByteArray data = reply->readAll();
		switch(error_code)
		{
		case 0:
			{
				if(!data.isEmpty())
				{
					QJsonDocument doc = QJsonDocument::fromJson(data);
					QJsonObject js_in = doc.object();
					strAccessToken = js_in["access_token"].toString();
					status = Status::Success;
				}
				break;
			}

		case 302:
			{
				if(data.contains("code doesn't exist or has expired"))
				{
					errorInfo = QString("ERROR. Unexpected status %1").arg(error_code);
					errorInfo += data;
					status = Status::InvalidTokenOrCode;
					break;
				}

				// Fall-through for other errors
			}

		default:
			{
				errorInfo = QString("ERROR. Unexpected status %1").arg(error_code);
				errorInfo += data;
				errorInfo += "\n";
				errorInfo += reply->errorString();
				status = Status::ConnectionError;
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
	else
	{
		m_AccessToken = strAccessToken;
	}

	return status;
}

RecipebookDropbox::Status RecipebookDropbox::setAccessToken(QString strAccessToken)
{
	if(strAccessToken.isEmpty())
	{
		return Status::InvalidTokenOrCode;
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

	Status status = Status::ConnectionError;
	QObject::connect(reply, &QNetworkReply::finished, [&]()
	{
		int error_code = reply->error();
		QByteArray data = reply->readAll();
		switch(error_code)
		{
		case 0:
			{
				if(data.isEmpty())
				{
					errorInfo = "Verification failed: no data received!";
					status = Status::ConnectionError;
				}
				else
				{
					QJsonDocument doc = QJsonDocument::fromJson(data);
					QJsonObject js_in = doc.object();
					if(js_in["result"].toString() != "recipebook-TEST")
					{
						errorInfo = "Verification failed: invalid data!";
						status = Status::ConnectionError;
					}
					else
					{
						status = Status::Success;
					}
				}
				break;
			}

		case 204:
			{
				if(data.contains("invalid_access_token"))
				{
					errorInfo = QString("ERROR. Unexpected status %1").arg(error_code);
					errorInfo += data;
					status = Status::InvalidTokenOrCode;
					break;
				}

				// Fall-through for other errors
			}

		default:
			{
				errorInfo = QString("ERROR. Unexpected status %1").arg(error_code);
				errorInfo += data;
				errorInfo += "\n";
				errorInfo += reply->errorString();
				status = Status::ConnectionError;
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

void RecipebookDropbox::setFileId(QString fileId)
{
	m_FileId = fileId;

	m_DropboxFileRev.clear();
	m_DropboxFileId.clear();
}

RecipebookDropbox::Status RecipebookDropbox::getCurrentFileContent(QByteArray& fileContent)
{
	if(m_AccessToken.isEmpty())
	{
		return Status::InvalidTokenOrCode;
	}
	if(m_FileId.isEmpty())
	{
		return Status::InvalidFileId;
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

	Status status = Status::ConnectionError;
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

				status = Status::Success;
				break;
			}

			case 206:
			{
				if(data.contains("path/not_found"))
				{
					errorInfo = QString("ERROR. Unexpected status %1").arg(error_code);
					errorInfo += data;
					status = Status::FileNotFound;
					break;
				}

				// Fall-through for other errors
			}

			default:
			{
				errorInfo = QString("ERROR. Unexpected status %1").arg(error_code);
				errorInfo += data;
				status = Status::ConnectionError;
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

RecipebookDropbox::Status RecipebookDropbox::updateFileContent(QByteArray fileContent)
{
	if(m_AccessToken.isEmpty())
	{
		return Status::InvalidTokenOrCode;
	}
	if(m_FileId.isEmpty())
	{
		return Status::InvalidFileId;
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

	Status status = Status::ConnectionError;
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
				status = Status::Success;
				break;
			}

			case 206:
			{
				if(data.contains("path/conflict/file"))
				{
					errorInfo = QString("ERROR. Unexpected status %1").arg(error_code);
					errorInfo += data;
					status = Status::FileChangedOnServer;
					break;
				}

				// Fall-through for other errors
			}

			default:
			{
				errorInfo = QString("ERROR. Unexpected status %1").arg(error_code);
				errorInfo += reply->readAll();
				status = Status::ConnectionError;
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
