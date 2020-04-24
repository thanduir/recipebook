#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "RecipeBookDataHandler.h"
#include "RecipeBookSettings.h"

bool setupNameLists(QQmlContext* context, recipebook::UI::RecipeBookDataHandler& dataHandler)
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

    context->setContextProperty("modelCategories", &dataHandler.getCategoriesModel());
    context->setContextProperty("modelSortOrder", &dataHandler.getSortOrderModel());

    context->setContextProperty("modelProvenance", &dataHandler.getProvenanceModel());
    context->setContextProperty("modelSortOrders", &dataHandler.getSortOrdersModel());

    context->setContextProperty("modelIngredients", &dataHandler.getIngredientsModel());
    context->setContextProperty("filterModelIngredients", &dataHandler.getIngredientsFilterModel());

    context->setContextProperty("alternativesGroups", &dataHandler.getAlternativesGroups());
    context->setContextProperty("alternativesTypes", &dataHandler.getAlternativesTypes());

    context->setContextProperty("modelRecipes", &dataHandler.getRecipesModel());
    context->setContextProperty("filterModelRecipes", &dataHandler.getRecipesFilterModel());

    context->setContextProperty("modelRecipeItems", &dataHandler.getRecipeItemsModel());

    return true;
}

bool setupConnections(QObject* pRoot, recipebook::UI::RecipeBookDataHandler& dataHandler)
{
    QObject* fileDialogSaveAs = pRoot->findChild<QObject*>(QStringLiteral("fileDialogExport"));
    QObject::connect(fileDialogSaveAs, SIGNAL(onSaveAs(QString)),
                     &dataHandler, SLOT(slotSaveAs(QString)));
    return true;
}

int main(int argc, char *argv[])
{
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    recipebook::UI::RecipeBookDataHandler dataHandler;
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
