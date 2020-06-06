#include "RBDialogInterface.h"
#include <QQmlApplicationEngine>

using namespace recipebook;

RBDialogInterface::RBDialogInterface()
{
}

QObject* RBDialogInterface::getDlgObject() const
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

	return rootObject->findChild<QObject*>(QStringLiteral("appDlgInterface"));
}

void RBDialogInterface::showMessageBox(QString strTitle, QString strMessage, DlgType type) const
{
	QObject* dlgObject = getDlgObject();
	if(dlgObject == nullptr)
	{
		return;
	}

	QMetaObject::invokeMethod(dlgObject, "showMessageBox",
							  Q_ARG(QString, strTitle),
							  Q_ARG(QString, strMessage),
							  Q_ARG(bool, type == DlgType::Error));
}

void RBDialogInterface::lockUI() const
{
	QObject* dlgObject = getDlgObject();
	if(dlgObject == nullptr)
	{
		return;
	}

	QMetaObject::invokeMethod(dlgObject, "lockUI");
}

void RBDialogInterface::unlockUI() const
{
	QObject* dlgObject = getDlgObject();
	if(dlgObject == nullptr)
	{
		return;
	}

	QMetaObject::invokeMethod(dlgObject, "unlockUI");
}
