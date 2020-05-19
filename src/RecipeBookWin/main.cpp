#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "RecipeBookUIData.h"
#include "RecipeBookSettings.h"

bool setupNameLists(QQmlContext* context, recipebook::UI::RecipeBookUIData& dataHandler)
{
	if(context == nullptr)
	{
		return false;
	}

	context->setContextProperty("recipeBookSettings", &dataHandler.getRecipeBookSettings());

	context->setContextProperty("unitNames", dataHandler.getAllUnitNames());
	context->setContextProperty("unitNamesShort", dataHandler.getAllUnitShortNames());
    
	context->setContextProperty("sizeNames", dataHandler.getAllSizeNames());
	context->setContextProperty("statusNames", dataHandler.getAllStatusNames());

	context->setContextProperty("shoppingListOrderingNames", dataHandler.getAllShoppingListOrderingNames());

	context->setContextProperty("modelCategories", &dataHandler.getCategoriesModel());
	context->setContextProperty("modelSortOrder", &dataHandler.getSortOrderModel());

	context->setContextProperty("modelSortOrders", &dataHandler.getSortOrdersModel());

	context->setContextProperty("modelIngredients", &dataHandler.getIngredientsModel());
	context->setContextProperty("filterModelIngredients", &dataHandler.getIngredientsFilterModel());

	context->setContextProperty("alternativesGroups", &dataHandler.getAlternativesGroups());
	context->setContextProperty("alternativesTypes", &dataHandler.getAlternativesTypes());

	context->setContextProperty("modelRecipes", &dataHandler.getRecipesModel());
	context->setContextProperty("filterModelRecipes", &dataHandler.getRecipesFilterModel());

	context->setContextProperty("modelRecipeItems", &dataHandler.getRecipeItemsModel());
	context->setContextProperty("filterModelRecipeItems", &dataHandler.getRecipeItemsFilterModel());

	context->setContextProperty("modelShoppingRecipes", &dataHandler.getShoppingRecipesModel());
	context->setContextProperty("modelShoppingListItems", &dataHandler.getShoppingListItemsModel());

	context->setContextProperty("modelGoShopping", &dataHandler.getGoShoppingModel());

	return true;
}

bool setupConnections(QObject* pRoot, recipebook::UI::RecipeBookUIData& dataHandler)
{
	QObject* fileDialogExport = pRoot->findChild<QObject*>(QStringLiteral("fileDialogExport"));
	QObject::connect(fileDialogExport, SIGNAL(onExport(QString)),
						&dataHandler, SLOT(slotExport(QString)));

	QObject* fileDialogImport = pRoot->findChild<QObject*>(QStringLiteral("fileDialogImport"));
	QObject::connect(fileDialogImport, SIGNAL(onImport(QString)),
						&dataHandler, SLOT(slotImport(QString)));

	//QObject* mainWindow = pRoot->findChild<QObject*>(QStringLiteral("recipeBookMainWindow"));
	QObject::connect(pRoot, SIGNAL(onClosingRecipeBook()),
						&dataHandler, SLOT(slotSave()));
	return true;
}

int main(int argc, char *argv[])
{
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication app(argc, argv);

	QQmlApplicationEngine engine;

	recipebook::UI::RecipeBookUIData dataHandler;
	QQmlContext* ctxt = engine.rootContext();
	if(!setupNameLists(ctxt, dataHandler))
	{
		return -1;
	}

	engine.load(QUrl(QStringLiteral("qrc:/recipebook.qml")));
	if(engine.rootObjects().isEmpty())
	{
		return -1;
	}

	QObject* rootObject = engine.rootObjects().first();
	if(!setupConnections(rootObject, dataHandler))
	{
		return -1;
	}

	return app.exec();
}
