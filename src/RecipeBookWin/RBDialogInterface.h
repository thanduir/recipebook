#ifndef RECIPEBOOK_DIALOG_INTERFACE_H
#define RECIPEBOOK_LATEX_EXPORTER_H

#include <QString>

class QQmlApplicationEngine;

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

	private:
		const QQmlApplicationEngine* m_pEngine = nullptr;
	};
}

#endif
