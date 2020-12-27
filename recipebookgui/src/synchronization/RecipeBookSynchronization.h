#ifndef RECIPEBOOK_SYNCHRONIZATION_H
#define RECIPEBOOK_SYNCHRONIZATION_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <synchronization/RecipebookDropbox.h>
#include "RecipeBookChanges.h"
#include "RecipeBookConflicts.h"

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

		void checkSyncReminder();

		void performTwoWayMerge(bool bKeepServerSide);

	signals:
		void signalLocalDataUpdated();

	private:
		void performMerge();		
		void performThreeWayMerge();

		bool readServerFile();
		bool readLocalFile();
		bool readBaseFile();

		void clearShoppingLists();

		bool uploadFile(QSharedPointer<RecipeBook> spFile);
		bool updateBase(QSharedPointer<RecipeBook> spFile);
		bool updateLocal(QSharedPointer<RecipeBook> spFile);

		void cleanUp();

		QObject* getDlgObject() const;

	private:
		RBDataHandler&						m_rRBDataHandler;
		RecipeBookSettings&					m_rSettings;
		const RBDialogInterface&			m_rDlgInterface;

		const QQmlApplicationEngine*		m_pEngine = nullptr;

		synchronization::RecipebookDropbox	m_rbDropbox;

		QSharedPointer<RecipeBook>			m_spRBServer;
		QSharedPointer<RecipeBook>			m_spRBLocal;
		QSharedPointer<RecipeBook>			m_spRBBase;

		RecipeBookChanges					m_changesLocal;
		RecipeBookChanges					m_changesServer;
		RecipeBookConflicts					m_Conflicts;
	};
}

#endif
