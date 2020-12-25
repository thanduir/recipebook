#include "RecipeBookConflicts.h"
#include "data/RecipeBook.h"
#include "data/Category.h"
#include "data/AlternativesType.h"
#include "data/Ingredient.h"
#include "data/Recipe.h"
#include "data/RecipeItem.h"
#include "data/RecipeBookConfiguration.h"

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

	bool recipeChangeConflicting(const Recipe& rLocal, const Recipe& rServer, const Recipe& rBase)
	{
		if(rLocal.getNumberOfPersons() != rBase.getNumberOfPersons()
		   && rServer.getNumberOfPersons() != rBase.getNumberOfPersons())
		{
			return true;
		}

		if(rLocal.getShortDescription() != rBase.getShortDescription()
		   && rServer.getShortDescription() != rBase.getShortDescription())
		{
			return true;
		}

		if(rLocal.getRecipeText() != rBase.getRecipeText()
		   && rServer.getRecipeText() != rBase.getRecipeText())
		{
			return true;
		}

		if(rLocal.getCookingTime() != rBase.getCookingTime()
		   && rServer.getCookingTime() != rBase.getCookingTime())
		{
			return true;
		}

		return false;
	}

	bool recipeItemChangeConflicting(const RecipeItem& rLocal, const RecipeItem& rServer, const RecipeItem& rBase)
	{
		if(rLocal.getIngredient().getName() != rBase.getIngredient().getName()
		   && rServer.getIngredient().getName() != rBase.getIngredient().getName())
		{
			return true;
		}

		if(rLocal.getAmount() != rBase.getAmount()
		   && rServer.getAmount() != rBase.getAmount())
		{
			return true;
		}

		if(rLocal.getAdditionalInfo() != rBase.getAdditionalInfo()
		   && rServer.getAdditionalInfo() != rBase.getAdditionalInfo())
		{
			return true;
		}

		if(rLocal.getSize() != rBase.getSize()
		   && rServer.getSize() != rBase.getSize())
		{
			return true;
		}

		if(rLocal.isOptional() != rBase.isOptional()
		   && rServer.isOptional() != rBase.isOptional())
		{
			return true;
		}

		if(rLocal.hasAlternativesGroup() && rServer.hasAlternativesGroup())
		{
			if(rBase.hasAlternativesGroup())
			{
				if(rLocal.getAlternativesGroup().getName() != rBase.getAlternativesGroup().getName()
				   && rServer.getAlternativesGroup().getName() != rBase.getAlternativesGroup().getName())
				{
					return true;
				}
			}
			else
			{
				if(rLocal.getAlternativesGroup().getName() != rServer.getAlternativesGroup().getName())
				{
					return true;
				}
			}
		}
		else if(rLocal.hasAlternativesGroup() && !rServer.hasAlternativesGroup())
		{
			if(rBase.hasAlternativesGroup()
			   && rLocal.getAlternativesGroup().getName() != rBase.getAlternativesGroup().getName())
			{
				return true;
			}
		}
		else if(!rLocal.hasAlternativesGroup() && rServer.hasAlternativesGroup())
		{
			if(rBase.hasAlternativesGroup()
			   && rServer.getAlternativesGroup().getName() != rBase.getAlternativesGroup().getName())
			{
				return true;
			}
		}

		return false;
	}
}

RecipeBookConflicts::RecipeBookConflicts()
{
}

bool RecipeBookConflicts::hasConflicts()
{
	return m_CategoryConflicts.size() > 0
		|| m_SortOrderConflicts.size() > 0
		|| m_AlternativesTypesConflicts.size() > 0
		|| m_IngredientsConflicts.size() > 0

		|| m_RecipesConflicts.size() > 0
		|| m_RecipeItemConflicts.size() > 0

		|| m_RBConfigurationsConflicts.size() > 0;
}

void RecipeBookConflicts::clear()
{
	m_CategoryConflicts.clear();
	m_SortOrderConflicts.clear();
	m_AlternativesTypesConflicts.clear();
	m_IngredientsConflicts.clear();

	m_RecipesConflicts.clear();
	m_RecipeItemConflicts.clear();

	m_RBConfigurationsConflicts.clear();
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
	findRecipesConflicts(spBase, spLocal, rChangesLocal, spServer, rChangesServer);
	findRBConfigurationsConflicts(spBase, spLocal, rChangesLocal, spServer, rChangesServer);
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
											   const RecipeBookChanges& rChangesLocal,
											   const QSharedPointer<RecipeBook> spServer,
											   const RecipeBookChanges& rChangesServer)
{
	// RemovedLocal_ChangedServer
	for(QString strElement : rChangesLocal.getRecipesChanges().m_Removed)
	{
		if(rChangesServer.getRecipesChanges().m_Changed.contains(strElement))
		{
			m_RecipesConflicts.append(Conflict(strElement, RecipeBookConflicts::ConflictType::RemovedLocal_ChangedServer));
		}
	}

	// ChangedLocal_RemovedServer and ChangedBoth
	for(QString strElement : rChangesLocal.getRecipesChanges().m_Changed)
	{
		if(rChangesServer.getRecipesChanges().m_Removed.contains(strElement))
		{
			m_RecipesConflicts.append(Conflict(strElement, RecipeBookConflicts::ConflictType::ChangedLocal_RemovedServer));
		}
		else if(rChangesServer.getRecipesChanges().m_Changed.contains(strElement))
		{
			const Recipe& rRecipeLocal = spLocal->getRecipe(strElement);
			const Recipe& rRecipeServer = spServer->getRecipe(strElement);
			const Recipe& rRecipeBase = spBase->getRecipe(strElement);
			if(recipeChangeConflicting(rRecipeLocal, rRecipeServer, rRecipeBase))
			{
				m_RecipesConflicts.append(Conflict(strElement, RecipeBookConflicts::ConflictType::ChangedBoth));
			}

			findRecipeItemConflicts(rRecipeBase, 
									rRecipeLocal, 
									rChangesLocal.getRecipeChanges(strElement), 
									rRecipeServer, 
									rChangesServer.getRecipeChanges(strElement));
		}
	}

	// AddedBothDifferingValues
	for(QString strElement : rChangesLocal.getRecipesChanges().m_Added)
	{
		// No conflict if the added types are equal
		if(rChangesServer.getRecipesChanges().m_Added.contains(strElement)
		   && spLocal->getRecipe(strElement) != spServer->getRecipe(strElement))
		{
			m_RecipesConflicts.append(Conflict(strElement, RecipeBookConflicts::ConflictType::AddedBothDifferingValues));
		}
	}
}

void RecipeBookConflicts::findRecipeItemConflicts(const Recipe& rBase,
												  const Recipe& rLocal,
												  const RecipeBookChanges::ItemChanges& rChangesLocal,
												  const Recipe& rServer,
												  const RecipeBookChanges::ItemChanges& rChangesServer)
{
	QString strRecipe = rBase.getName();

	// RemovedLocal_ChangedServer
	for(QString strElement : rChangesLocal.m_Removed)
	{
		if(rChangesServer.m_Changed.contains(strElement))
		{
			m_RecipeItemConflicts.append(Conflict(strElement, RecipeBookConflicts::ConflictType::RemovedLocal_ChangedServer, strRecipe));
		}
	}

	// ChangedLocal_RemovedServer and ChangedBoth
	for(QString strElement : rChangesLocal.m_Changed)
	{
		if(rChangesServer.m_Removed.contains(strElement))
		{
			m_RecipeItemConflicts.append(Conflict(strElement, RecipeBookConflicts::ConflictType::ChangedLocal_RemovedServer, strRecipe));
		}
		else if(rChangesServer.m_Changed.contains(strElement))
		{
			int indexLocal = rLocal.getRecipeItemIndex(strElement);
			int indexServer = rServer.getRecipeItemIndex(strElement);
			int indexBase = rBase.getRecipeItemIndex(strElement);

			const RecipeItem& rItemLocal = rLocal.getRecipeItemAt(indexLocal);
			const RecipeItem& rItemServer = rServer.getRecipeItemAt(indexServer);
			const RecipeItem& rItemBase = rBase.getRecipeItemAt(indexBase);
			if(recipeItemChangeConflicting(rItemLocal, rItemServer, rItemBase))
			{
				m_RecipeItemConflicts.append(Conflict(strElement, RecipeBookConflicts::ConflictType::ChangedBoth, strRecipe));
			}
		}
	}

	// AddedBothDifferingValues
	for(QString strElement : rChangesLocal.m_Added)
	{
		int indexLocal = rLocal.getRecipeItemIndex(strElement);
		int indexServer = rServer.getRecipeItemIndex(strElement);

		// No conflict if the added types are equal
		if(rChangesServer.m_Added.contains(strElement)
		   && rLocal.getRecipeItemAt(indexLocal) != rServer.getRecipeItemAt(indexServer))
		{
			m_RecipeItemConflicts.append(Conflict(strElement, RecipeBookConflicts::ConflictType::AddedBothDifferingValues, strRecipe));
		}
	}
}

void RecipeBookConflicts::findRBConfigurationsConflicts(const QSharedPointer<RecipeBook> spBase, 
														const QSharedPointer<RecipeBook> spLocal,
														const RecipeBookChanges& rChangesLocal,
														const QSharedPointer<RecipeBook> spServer,
														const RecipeBookChanges& rChangesServer)
{
	// RemovedLocal_ChangedServer
	for(QString strElement : rChangesLocal.getRBConfigurationsChanges().m_Removed)
	{
		if(rChangesServer.getRBConfigurationsChanges().m_Changed.contains(strElement))
		{
			m_RBConfigurationsConflicts.append(Conflict(strElement, RecipeBookConflicts::ConflictType::RemovedLocal_ChangedServer));
		}
	}

	// ChangedLocal_RemovedServer and ChangedBoth
	for(QString strElement : rChangesLocal.getRBConfigurationsChanges().m_Changed)
	{
		if(rChangesServer.getRBConfigurationsChanges().m_Removed.contains(strElement))
		{
			m_RBConfigurationsConflicts.append(Conflict(strElement, RecipeBookConflicts::ConflictType::ChangedLocal_RemovedServer));
		}
		else if(rChangesServer.getRBConfigurationsChanges().m_Changed.contains(strElement))
		{
			const RecipeBookConfiguration& rConfigLocal = spLocal->getConfiguration(strElement);
			const RecipeBookConfiguration& rConfigServer = spServer->getConfiguration(strElement);
			if(rConfigLocal != rConfigServer)
			{
				// Hint: For now, configurations are look at as a whole. Needs to be changed here if this is not as it should be
				m_RBConfigurationsConflicts.append(Conflict(strElement, RecipeBookConflicts::ConflictType::ChangedBoth));
			}
		}
	}

	// AddedBothDifferingValues
	for(QString strElement : rChangesLocal.getRBConfigurationsChanges().m_Added)
	{
		// No conflict if the added types are equal
		if(rChangesServer.getRBConfigurationsChanges().m_Added.contains(strElement)
		   && spLocal->getConfiguration(strElement) != spServer->getConfiguration(strElement))
		{
			m_RBConfigurationsConflicts.append(Conflict(strElement, RecipeBookConflicts::ConflictType::AddedBothDifferingValues));
		}
	}
}
