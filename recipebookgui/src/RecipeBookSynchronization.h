#ifndef RECIPEBOOK_SYNCHRONIZATION_H
#define RECIPEBOOK_SYNCHRONIZATION_H

#include <QObject>
#include <synchronization/RecipebookDropbox.h>

namespace recipebook
{
	class RBDataHandler;
	class RecipeBookSettings;

	class RecipeBookSynchronization final : public QObject
	{
		Q_OBJECT

	public:
		RecipeBookSynchronization(RBDataHandler& rRBDataHandler,
								  RecipeBookSettings& rSettings);

	public slots:
		void execute();

	private:
		RBDataHandler&						m_rRBDataHandler;
		RecipeBookSettings&					m_rSettings;

		synchronization::RecipebookDropbox	m_rbDropbox;
	};
}

#endif
