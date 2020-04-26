#ifndef RECIPEBOOK_UI_DATA_H
#define RECIPEBOOK_UI_DATA_H

#include <QtGlobal>
#include "RBDataHandler.h"
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

	public slots:
		void slotSave();
		void slotExport(QString strFileURL);
		void slotImport(QString strFileURL);
		void slotResetData();

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

		QAtomicInt m_SaveLock;
	};
}

#endif
