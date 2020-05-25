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
		RBDialogInterface(const QQmlApplicationEngine& rEngine);

		void showMessageBox(QString strTitle, QString strMessage, DlgType type) const;

	private:
		const QQmlApplicationEngine& m_rEngine;
	};
}

#endif
