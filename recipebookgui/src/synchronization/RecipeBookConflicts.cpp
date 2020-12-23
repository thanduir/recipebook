#include "RecipeBookConflicts.h"
#include "data/RecipeBook.h"
#include "data/Category.h"
#include "data/AlternativesType.h"
#include "data/Ingredient.h"

using namespace recipebook;

namespace
{
	// Verify that the changes are actually conflicting! -> no conflict if types equal or changes independent
	bool ingredientChangeConflicting(const Ingredient& rLocal, const Ingredient& rServer, const Ingredient& rBase)
	{
		if(rLocal.getCategory().getName() != rBase.getCategory().getName()
		   && rServer.getCategory().getName() != rBase.getCategory().getName())
		{
			return true;
		}

		if(rLocal.getDefaultUnit() != rBase.getDefaultUnit()
		   && rServer.getDefaultUnit() != rBase.getDefaultUnit())
		{
			return true;
		}

		// TODO: PROVENANCES!

		return false;
	}
}

RecipeBookConflicts::RecipeBookConflicts()
{
}

void RecipeBookConflicts::clear()
{
	//  TODO
}

void RecipeBookConflicts::findConflicts(const QSharedPointer<RecipeBook> spBase, 
										const QSharedPointer<RecipeBook> spLocal,
										const RecipeBookChanges& rChangesLocal,
										const QSharedPointer<RecipeBook> spServer,
										const RecipeBookChanges& rChangesServer)
{
	clear();

	findCategoryConflicts(rChangesLocal.getCategoryChanges(), rChangesServer.getCategoryChanges());
	findSortOrderConflicts(spLocal, rChangesLocal.getSortOrderChanges(), spServer, rChangesServer.getSortOrderChanges());
	findAlternativesTypesConflicts(spLocal, rChangesLocal.getAlternativeTypesChanges(), spServer, rChangesServer.getAlternativeTypesChanges());
	findIngredientsConflicts(spBase, spLocal, rChangesLocal.getIngredientsChanges(), spServer, rChangesServer.getIngredientsChanges());
	findRecipesConflicts(spBase, spLocal, rChangesLocal.getRecipesChanges(), spServer, rChangesServer.getRecipesChanges());
	findRBConfigurationsConflicts(spBase, spLocal, rChangesLocal.getRBConfigurationsChanges(), spServer, rChangesServer.getRBConfigurationsChanges());
}

void RecipeBookConflicts::findCategoryConflicts(const RecipeBookChanges::ItemChanges& rChangesLocal,
												const RecipeBookChanges::ItemChanges& rChangesServer)
{
	// There are no relevant conflicts for now
	Q_UNUSED(rChangesLocal);
	Q_UNUSED(rChangesServer);
}

void RecipeBookConflicts::findSortOrderConflicts(const QSharedPointer<RecipeBook> spLocal,
												 const RecipeBookChanges::ItemChanges& rChangesLocal,
												 const QSharedPointer<RecipeBook> spServer,
												 const RecipeBookChanges::ItemChanges& rChangesServer)
{
	// RemovedLocal_ChangedServer
	for(QString strElement : rChangesLocal.m_Removed)
	{
		if(rChangesServer.m_Changed.contains(strElement))
		{
			m_SortOrderConflicts.append(Conflict(strElement, RecipeBookConflicts::ConflictType::RemovedLocal_ChangedServer));
		}
	}

	// ChangedLocal_RemovedServer and ChangedBoth
	for(QString strElement : rChangesLocal.m_Changed)
	{
		if(rChangesServer.m_Removed.contains(strElement))
		{
			m_SortOrderConflicts.append(Conflict(strElement, RecipeBookConflicts::ConflictType::ChangedLocal_RemovedServer));
		}
		else if(rChangesServer.m_Changed.contains(strElement))
		{
			// TODO: Verify that the changes are actually conflicting! (e.g. if the sortorders are not equal. Anything else possible here? Maybe newly added categories inserted at different positions?)
			//		-> test equality up to newly added elements (unless the added element are the same, then they need to be at the same position)
			m_SortOrderConflicts.append(Conflict(strElement, RecipeBookConflicts::ConflictType::ChangedBoth));
		}
	}

	// AddedBothDifferingValues
	for(QString strElement : rChangesLocal.m_Added)
	{
		if(rChangesServer.m_Added.contains(strElement))
		{
			// TODO: Verify that the changes are actually conflicting! (e.g. if the sortorders are not equal. Anything else possible here? Maybe newly added categories inserted at different positions?)
			//		-> test equality up to newly added elements (unless the added element are the same, then they need to be at the same position)
			m_SortOrderConflicts.append(Conflict(strElement, RecipeBookConflicts::ConflictType::AddedBothDifferingValues));
		}
	}
}

void RecipeBookConflicts::findAlternativesTypesConflicts(const QSharedPointer<RecipeBook> spLocal,
														 const RecipeBookChanges::ItemChanges& rChangesLocal,
														 const QSharedPointer<RecipeBook> spServer,
														 const RecipeBookChanges::ItemChanges& rChangesServer)
{
	// RemovedLocal_ChangedServer
	for(QString strElement : rChangesLocal.m_Removed)
	{
		if(rChangesServer.m_Changed.contains(strElement))
		{
			m_AlternativesTypesConflicts.append(Conflict(strElement, RecipeBookConflicts::ConflictType::RemovedLocal_ChangedServer));
		}
	}

	// ChangedLocal_RemovedServer and ChangedBoth
	for(QString strElement : rChangesLocal.m_Changed)
	{
		if(rChangesServer.m_Removed.contains(strElement))
		{
			m_AlternativesTypesConflicts.append(Conflict(strElement, RecipeBookConflicts::ConflictType::ChangedLocal_RemovedServer));
		}
		else if(rChangesServer.m_Changed.contains(strElement))
		{
			// No conflict if the changes are equivalent
			if(spLocal->getAlternativesType(strElement) != spServer->getAlternativesType(strElement))
			{
				m_AlternativesTypesConflicts.append(Conflict(strElement, RecipeBookConflicts::ConflictType::ChangedBoth));
			}
		}
	}

	// AddedBothDifferingValues
	for(QString strElement : rChangesLocal.m_Added)
	{
		// No conflict if the added types are equal
		if(rChangesServer.m_Added.contains(strElement)
		   && spLocal->getAlternativesType(strElement) != spServer->getAlternativesType(strElement))
		{
			m_AlternativesTypesConflicts.append(Conflict(strElement, RecipeBookConflicts::ConflictType::AddedBothDifferingValues));
		}
	}
}

void RecipeBookConflicts::findIngredientsConflicts(const QSharedPointer<RecipeBook> spBase, 
												   const QSharedPointer<RecipeBook> spLocal,
												   const RecipeBookChanges::ItemChanges& rChangesLocal,
												   const QSharedPointer<RecipeBook> spServer,
												   const RecipeBookChanges::ItemChanges& rChangesServer)
{
	// RemovedLocal_ChangedServer
	for(QString strElement : rChangesLocal.m_Removed)
	{
		if(rChangesServer.m_Changed.contains(strElement))
		{
			m_IngredientsConflicts.append(Conflict(strElement, RecipeBookConflicts::ConflictType::RemovedLocal_ChangedServer));
		}
	}

	// ChangedLocal_RemovedServer and ChangedBoth
	for(QString strElement : rChangesLocal.m_Changed)
	{
		if(rChangesServer.m_Removed.contains(strElement))
		{
			m_IngredientsConflicts.append(Conflict(strElement, RecipeBookConflicts::ConflictType::ChangedLocal_RemovedServer));
		}
		else if(rChangesServer.m_Changed.contains(strElement))
		{
			const Ingredient& rIngredientLocal = spLocal->getIngredient(strElement);
			const Ingredient& rIngredientServer = spServer->getIngredient(strElement);
			const Ingredient& rIngredientBase = spBase->getIngredient(strElement);
			if(ingredientChangeConflicting(rIngredientLocal, rIngredientServer, rIngredientBase))
			{
				m_IngredientsConflicts.append(Conflict(strElement, RecipeBookConflicts::ConflictType::ChangedBoth));
			}
		}
	}

	// AddedBothDifferingValues
	for(QString strElement : rChangesLocal.m_Added)
	{
		// No conflict if the added types are equal
		if(rChangesServer.m_Added.contains(strElement)
		   && spLocal->getIngredient(strElement) != spServer->getIngredient(strElement))
		{
			m_IngredientsConflicts.append(Conflict(strElement, RecipeBookConflicts::ConflictType::AddedBothDifferingValues));
		}
	}
}

void RecipeBookConflicts::findRecipesConflicts(const QSharedPointer<RecipeBook> spBase, 
											   const QSharedPointer<RecipeBook> spLocal,
											   const RecipeBookChanges::ItemChanges& rChangesLocal,
											   const QSharedPointer<RecipeBook> spServer,
											   const RecipeBookChanges::ItemChanges& rChangesServer)
{
	// TODO
	rChangesLocal;
	rChangesServer;
}

void RecipeBookConflicts::findRBConfigurationsConflicts(const QSharedPointer<RecipeBook> spBase, 
														const QSharedPointer<RecipeBook> spLocal,
														const RecipeBookChanges::ItemChanges& rChangesLocal,
														const QSharedPointer<RecipeBook> spServer,
														const RecipeBookChanges::ItemChanges& rChangesServer)
{
	// TODO
	rChangesLocal;
	rChangesServer;
}
