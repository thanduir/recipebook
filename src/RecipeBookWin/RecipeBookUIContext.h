#ifndef RECIPEBOOK_UI_DATA_H
#define RECIPEBOOK_UI_DATA_H

#include <QtGlobal>
#include <QQmlApplicationEngine>
#include <data/RBDataHandler.h>
#include "RecipeBookSettings.h"
#include "ShoppingListExporter.h"
#include "RecipeBookExporter.h"
#include "ListModelCategories.h"
#include "ListModelIngredients.h"
#include "FilterModelIngredients.h"
#include "SortModelSortOrder.h"
#include "ListModelSortOrders.h"
#include "ListModelAlternativesGroups.h"
#include "FilterModelAlternativesTypes.h"
#include "ListModelRecipes.h"
#include "FilterModelRecipes.h"
#include "ListModelRecipeItems.h"
#include "FilterModelRecipeItems.h"
#include "ListModelShoppingRecipes.h"
#include "ListModelShoppingListItems.h"
#include "ListModelGoShopping.h"
#include "ListModelRecipeBookConfigurations.h"
#include "ListModelRBConfigItems.h"
#include "FilterModelUnusedRecipes.h"
#include "uistringconverter.h"
#include "RBDialogInterface.h"

class QQmlContext;

namespace recipebook::UI
{
	class RecipeBookUIContext : public QObject
	{
		Q_OBJECT

	public:
		RecipeBookUIContext();

		bool setupQml();

	public slots:
		void slotSave();
		void slotExport(QString strFileURL);
		void slotImport(QString strFileURL);
		void slotResetData();
		void slotLoadDefaultData();

	signals:
		void signalDataReset();

	private:
		bool setupNameLists(QQmlContext* context);
		bool setupConnections(QObject* pRoot);

	private:
		RBDataHandler m_RBData;
		UIStringConverter m_Converter;

		RecipeBookSettings m_Settings;

		ListModelCategories m_ModelCategories;
		SortModelSortOrder m_ModelSortOrder;

		ListModelSortOrders m_ModelSortOrders;

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

		ListModelRecipeBookConfigurations m_ModelConfigurations;
		ListModelRBConfigItems m_ModelConfigItems;
		FilterModelUnusedRecipes m_FilterModelUnusedRecipes;

		RBDialogInterface m_DlgInterface;
		ShoppingListExporter m_ShoppingListExporter;
		RecipeBookExporter m_RecipeBookExporter;

		QQmlApplicationEngine m_Engine;

		QAtomicInt m_SaveLock;
	};
}

#endif
