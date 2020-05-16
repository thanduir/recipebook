#ifndef RECIPEBOOK_UI_DATA_H
#define RECIPEBOOK_UI_DATA_H

#include <QtGlobal>
#include <data/RBDataHandler.h>
#include "RecipeBookSettings.h"
#include "ListModelCategories.h"
#include "ListModelIngredients.h"
#include "FilterModelIngredients.h"
#include "FilterModelSortOrders.h"
#include "SortModelSortOrder.h"
#include "ListModelProvenance.h"
#include "ListModelAlternativesGroups.h"
#include "FilterModelAlternativesTypes.h"
#include "ListModelRecipes.h"
#include "FilterModelRecipes.h"
#include "ListModelRecipeItems.h"
#include "FilterModelRecipeItems.h"
#include "ListModelShoppingRecipes.h"
#include "ListModelShoppingListItems.h"
#include "ListModelGoShopping.h"
#include "uistringconverter.h"

namespace recipebook::UI
{
	class RecipeBookUIData : public QObject
	{
		Q_OBJECT

	public:
		RecipeBookUIData();

		RecipeBookSettings& getRecipeBookSettings();

		// Namelists
		QStringList getAllUnitNames() const;
		QStringList getAllUnitShortNames() const;
		QStringList getAllSizeNames() const;
		QStringList getAllStatusNames() const;
		QStringList getAllShoppingListOrderingNames() const;

		// DataModels
		ListModelCategories& getCategoriesModel();
		SortModelSortOrder& getSortOrderModel();
		ListModelProvenance& getProvenanceModel();
		FilterModelSortOrders& getSortOrdersModel();
		ListModelIngredients& getIngredientsModel();
		FilterModelIngredients& getIngredientsFilterModel();
		ListModelAlternativesGroups& getAlternativesGroups();
		FilterModelAlternativesTypes& getAlternativesTypes();
		ListModelRecipes& getRecipesModel();
		FilterModelRecipes& getRecipesFilterModel();
		ListModelRecipeItems& getRecipeItemsModel();
		FilterModelRecipeItems& getRecipeItemsFilterModel();
		ListModelShoppingRecipes& getShoppingRecipesModel();
		ListModelShoppingListItems& getShoppingListItemsModel();
		ListModelGoShopping& getGoShoppingModel();

	public slots:
		void slotSave();
		void slotExport(QString strFileURL);
		void slotImport(QString strFileURL);
		void slotResetData();
		void slotLoadDefaultData();

	signals:
		void signalDataReset();

	private:
		RBDataHandler m_RBData;
		UIStringConverter m_Converter;

		RecipeBookSettings m_Settings;

		ListModelCategories m_ModelCategories;
		SortModelSortOrder m_ModelSortOrder;

		ListModelProvenance m_ModelProvenance;
		FilterModelSortOrders m_ModelSortOrders;

		ListModelIngredients m_ModelIngredients;
		FilterModelIngredients m_FilterModelIngredients;

		ListModelAlternativesGroups m_AlternativesGroups;
		FilterModelAlternativesTypes m_AlternativesTypes;

		ListModelRecipes m_ModelRecipes;
		FilterModelRecipes m_FilterModelRecipes;

		ListModelRecipeItems m_ModelRecipeItems;
		FilterModelRecipeItems m_FilterModelRecipeItems;

		ListModelShoppingRecipes m_ModelShoppingRecipes;
		ListModelShoppingListItems m_ModelShoppingListItems;

		ListModelGoShopping m_ModelGoShopping;

		QAtomicInt m_SaveLock;
	};
}

#endif
