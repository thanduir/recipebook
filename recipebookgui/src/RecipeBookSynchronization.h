#ifndef RECIPEBOOK_SYNCHRONIZATION_H
#define RECIPEBOOK_SYNCHRONIZATION_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <synchronization/RecipebookDropbox.h>

namespace recipebook
{
	class RecipeBook;
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

		void generateAndSetUniqueFileId();
		void setServerFileId(QString strId);

	private:
		void performMerge();
		bool readServerFile();
		bool readCurrentFile();
		bool readBaseFile();

		void cleanUp();

		QObject* getDlgObject() const;

	private:
		RBDataHandler&						m_rRBDataHandler;
		RecipeBookSettings&					m_rSettings;
		const RBDialogInterface&			m_rDlgInterface;

		const QQmlApplicationEngine*		m_pEngine = nullptr;

		synchronization::RecipebookDropbox	m_rbDropbox;

		QSharedPointer<RecipeBook>			m_spRBServer;
		QSharedPointer<RecipeBook>			m_spRBCurrent;
		QSharedPointer<RecipeBook>			m_spRBBase;
	};
}

#endif
