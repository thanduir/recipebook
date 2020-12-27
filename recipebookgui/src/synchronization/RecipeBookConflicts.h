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

			Conflict(QString strElement, ConflictType type)
			: m_strElement(strElement), m_Type(type) {}
		};

		typedef QVector<Conflict> ConflictList;

	public:
		RecipeBookConflicts();

		void findConflicts(const QSharedPointer<RecipeBook> spBase, 
						   const QSharedPointer<RecipeBook> spLocal,
						   const RecipeBookChanges& rChangesLocal, 
						   const QSharedPointer<RecipeBook> spServer,
						   const RecipeBookChanges& rChangesServer);
		
		void clear();

	public slots:
		bool hasConflicts();

		const ConflictList getCategoryConflicts() const { return m_CategoryConflicts; }
		const ConflictList getSortOderConflicts() const { return m_SortOrderConflicts; }
		const ConflictList getAlternativeTypeConflicts() const { return m_AlternativesTypesConflicts; }
		const ConflictList getIngredientConflicts() const { return m_IngredientsConflicts; }

		const ConflictList getRecipeConflicts() const { return m_RecipesConflicts; }
		const QMap<QString, ConflictList> getRecipeItemConflicts() const { return m_RecipeItemConflicts; }
		
		const ConflictList getRBConfigurationConflicts() const { return m_RBConfigurationsConflicts; }

	private:
		void findCategoryConflicts(const RecipeBookChanges::ItemChanges& rChangesLocal, 
								   const RecipeBookChanges::ItemChanges& rChangesServer);
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
		ConflictList				m_CategoryConflicts;
		ConflictList				m_SortOrderConflicts;
		ConflictList				m_AlternativesTypesConflicts;
		ConflictList				m_IngredientsConflicts;

		ConflictList				m_RecipesConflicts;
		QMap<QString, ConflictList>	m_RecipeItemConflicts;

		ConflictList				m_RBConfigurationsConflicts;
	};
}

#endif
