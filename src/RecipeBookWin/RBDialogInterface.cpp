#include "RBDialogInterface.h"
#include <QQmlApplicationEngine>

using namespace recipebook;

RBDialogInterface::RBDialogInterface()
{
}

void RBDialogInterface::showMessageBox(QString strTitle, QString strMessage, DlgType type) const
{
	if(!m_pEngine)
	{
		return;
	}

	QObject* rootObject = m_pEngine->rootObjects().first();
	if(!rootObject)
	{
		return;
	}

	QObject* dlgObject = rootObject->findChild<QObject*>(QStringLiteral("appDlgInterface"));
	if(!dlgObject)
	{
		return;
	}

	QMetaObject::invokeMethod(dlgObject, "showMessageBox",
							  Q_ARG(QString, strTitle),
							  Q_ARG(QString, strMessage),
							  Q_ARG(bool, type == DlgType::Error));
}
