#ifndef RECIPEBOOK_SYNCHRONIZATION_H
#define RECIPEBOOK_SYNCHRONIZATION_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <synchronization/RecipebookDropbox.h>

namespace recipebook
{
	class RBDataHandler;
	class RecipeBookSettings;
	class RBDialogInterface;

	class RecipeBookSynchronization final : public QObject
	{
		Q_OBJECT

	public:
		RecipeBookSynchronization(RBDataHandler& rRBDataHandler,
								  RecipeBookSettings& rSettings,
								  const RBDialogInterface& rDlgInterface);

		void setQmlEngine(const QQmlApplicationEngine& rEngine) { m_pEngine = &rEngine; }

	public slots:
		void execute();
		void setAccessCode(QString accessCode);

	private:
		void performMerge();

		QObject* getDlgObject() const;

	private:
		RBDataHandler&						m_rRBDataHandler;
		RecipeBookSettings&					m_rSettings;
		const RBDialogInterface&			m_rDlgInterface;

		const QQmlApplicationEngine*		m_pEngine = nullptr;

		synchronization::RecipebookDropbox	m_rbDropbox;
	};
}

#endif
