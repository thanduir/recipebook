#ifndef RECIPEBOOK_UI_DATA_H
#define RECIPEBOOK_UI_DATA_H

#include <QtGlobal>
#include <QQmlApplicationEngine>
#include <data/RBDataHandler.h>
#include "RecipeBookSettings.h"
#include "RBTranslationManager.h"
#include "RBDialogInterface.h"
#include "uistringconverter.h"
#include "exporter/ShoppingListExporter.h"
#include "exporter/RecipeBookExporter.h"
#include "model/ListModelCategories.h"
#include "model/ListModelIngredients.h"
#include "model/FilterModelIngredients.h"
#include "model/SortModelSortOrder.h"
#include "model/ListModelSortOrders.h"
#include "model/ListModelAlternativesGroups.h"
#include "model/FilterModelAlternativesTypes.h"
#include "model/ListModelRecipes.h"
#include "model/FilterModelRecipes.h"
#include "model/ListModelRecipeItems.h"
#include "model/FilterModelRecipeItems.h"
#include "model/ListModelShoppingRecipes.h"
#include "model/ListModelShoppingListItems.h"
#include "model/ListModelGoShopping.h"
#include "model/ListModelRecipeBookConfigurations.h"
#include "model/ListModelRBConfigItems.h"
#include "model/FilterModelUnusedRecipes.h"

class QQmlContext;

namespace recipebook::UI
{
	class RecipeBookUIContext : public QObject
	{
		Q_OBJECT

	public:
		RecipeBookUIContext(bool bOnAndroid);

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
		bool m_bOnAndroid;

		RBDataHandler m_RBData;

		RecipeBookSettings m_Settings;
		RBTranslationManager m_Translations;
		UIStringConverter m_Converter;

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
