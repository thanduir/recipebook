#ifndef RECIPEBOOK_DIALOG_INTERFACE_H
#define RECIPEBOOK_DIALOG_INTERFACE_H

#include <QString>

class QQmlApplicationEngine;
class QObject;

namespace recipebook
{
	class RBDialogInterface
	{
	public:
		enum class DlgType
		{
			Information,
			Error
		};

	public:
		RBDialogInterface();

		void setQmlEngine(const QQmlApplicationEngine& rEngine) { m_pEngine = &rEngine; }

		void showMessageBox(QString strTitle, QString strMessage, DlgType type) const;

		void lockUI() const;
		void unlockUI() const;

	private:
		QObject* getDlgObject() const;

	private:
		const QQmlApplicationEngine* m_pEngine = nullptr;
	};
}

#endif
