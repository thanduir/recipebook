#ifndef RECIPEBOOK_CONFLICTS_H
#define RECIPEBOOK_CONFLICTS_H

#include <QObject>
#include "RecipeBookChanges.h"

namespace recipebook
{
	class RecipeBook;
	class Recipe;
	class RecipeBookConfiguration;
	class RecipeBookChanges;

	class RecipeBookConflicts final : public QObject
	{
		Q_OBJECT

	public:
		enum class ConflictType
		{
			RemovedLocal_ChangedServer,
			ChangedLocal_RemovedServer,
			ChangedBoth,
			AddedBothDifferingValues
		};

		struct Conflict
		{
			QString			m_strElement;
			ConflictType	m_Type;
			QString			m_strParent;

			Conflict(QString strElement, ConflictType type, QString strParent = QString())
			: m_strElement(strElement), m_Type(type), m_strParent(strParent) {}
		};

	public:
		RecipeBookConflicts();

		void findConflicts(const QSharedPointer<RecipeBook> spBase, 
						   const QSharedPointer<RecipeBook> spLocal,
						   const RecipeBookChanges& rChangesLocal, 
						   const QSharedPointer<RecipeBook> spServer,
						   const RecipeBookChanges& rChangesServer);

		bool hasConflicts();
		void clear();

	private:
		void findCategoryConflicts(const RecipeBookChanges::ItemChanges& rChangesLocal, const RecipeBookChanges::ItemChanges& rChangesServer);
		void findSortOrderConflicts(const QSharedPointer<RecipeBook> spLocal,
									const RecipeBookChanges::ItemChanges& rChangesLocal,
									const QSharedPointer<RecipeBook> spServer,
									const RecipeBookChanges::ItemChanges& rChangesServer);
		void findAlternativesTypesConflicts(const QSharedPointer<RecipeBook> spLocal,
											const RecipeBookChanges::ItemChanges& rChangesLocal,
											const QSharedPointer<RecipeBook> spServer,
											const RecipeBookChanges::ItemChanges& rChangesServer);
		void findIngredientsConflicts(const QSharedPointer<RecipeBook> spBase, 
									  const QSharedPointer<RecipeBook> spLocal,
									  const RecipeBookChanges::ItemChanges& rChangesLocal,
									  const QSharedPointer<RecipeBook> spServer,
									  const RecipeBookChanges::ItemChanges& rChangesServer);
		
		void findRecipesConflicts(const QSharedPointer<RecipeBook> spBase,
								  const QSharedPointer<RecipeBook> spLocal,
								  const RecipeBookChanges& rChangesLocal,
								  const QSharedPointer<RecipeBook> spServer,
								  const RecipeBookChanges& rChangesServer);

		void findRecipeItemConflicts(const Recipe& rBase,
									 const Recipe& rLocal,
									 const RecipeBookChanges::ItemChanges& rChangesLocal,
									 const Recipe& rServer,
									 const RecipeBookChanges::ItemChanges& rChangesServer);

		void findRBConfigurationsConflicts(const QSharedPointer<RecipeBook> spBase, 
										   const QSharedPointer<RecipeBook> spLocal,
										   const RecipeBookChanges& rChangesLocal,
										   const QSharedPointer<RecipeBook> spServer,
										   const RecipeBookChanges& rChangesServer);


	private:
		QVector<Conflict>	m_CategoryConflicts;
		QVector<Conflict>	m_SortOrderConflicts;
		QVector<Conflict>	m_AlternativesTypesConflicts;
		QVector<Conflict>	m_IngredientsConflicts;	

		QVector<Conflict>	m_RecipesConflicts;
		QVector<Conflict>	m_RecipeItemConflicts;

		QVector<Conflict>	m_RBConfigurationsConflicts;
	};
}

#endif
