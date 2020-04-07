#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "RecipeBookDataHandler.h"

bool setupNameLists(QQmlContext* context, recipebook::UI::RecipeBookDataHandler& dataHandler)
{
    if(context == nullptr)
    {
        return false;
    }

    context->setContextProperty("unitNames", QVariant::fromValue(dataHandler.getAllUnitNames()));
    context->setContextProperty("sizeNames", QVariant::fromValue(dataHandler.getAllSizeNames()));
    context->setContextProperty("statusNames", QVariant::fromValue(dataHandler.getAllStatusNames()));

    context->setContextProperty("modelCategories", &dataHandler.getCategoriesModel());
    context->setContextProperty("modelSortOrder", &dataHandler.getSortOrderModel());

    context->setContextProperty("modelProvenance", &dataHandler.getProvenanceModel());
    context->setContextProperty("modelSortOrders", &dataHandler.getSortOrdersModel());

    context->setContextProperty("modelIngredients", &dataHandler.getIngredientsModel());

    return true;
}

bool setupConnections(QObject* pRoot, recipebook::UI::RecipeBookDataHandler& dataHandler)
{
    QObject* fileDialogSaveAs = pRoot->findChild<QObject*>(QStringLiteral("fileDialogSaveAs"));
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
