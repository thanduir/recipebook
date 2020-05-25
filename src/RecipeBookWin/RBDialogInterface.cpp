#include "RBDialogInterface.h"
#include <QQmlApplicationEngine>

using namespace recipebook;

RBDialogInterface::RBDialogInterface(const QQmlApplicationEngine& rEngine)
:	m_rEngine(rEngine)
{
}

void RBDialogInterface::showMessageBox(QString strTitle, QString strMessage, DlgType type) const
{
	QObject* rootObject = m_rEngine.rootObjects().first();
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
