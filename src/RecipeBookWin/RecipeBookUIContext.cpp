#include "RecipeBookUIContext.h"
#include <QtGlobal>
#include <QDir>
#include <QTimer>
#include <QUrl>
#include <QFileInfo>
#include <QQmlContext>
#include "serialization/RecipeBookSerializerFactory.h"
#include "data/Size.h"
#include "data/Status.h"
#include "data/Unit.h"

using namespace recipebook::UI;
using namespace recipebook::serialization;

const int c_SaveIntervalSeconds = 60;

RecipeBookUIContext::RecipeBookUIContext()
:	m_RBData(),
	m_Converter(),
	m_Settings(),
	m_ModelCategories(m_RBData),
	m_ModelSortOrder(m_RBData),
	m_ModelSortOrders(m_RBData, m_Converter),
	m_ModelIngredients(m_RBData, m_Settings, m_Converter),
	m_FilterModelIngredients(),
	m_AlternativesGroups(m_RBData, m_Converter),
	m_AlternativesTypes(),
	m_ModelRecipes(m_RBData, m_Settings),
	m_FilterModelRecipes(),
	m_ModelRecipeItems(m_RBData, m_Converter),
	m_FilterModelRecipeItems(),
	m_ModelShoppingRecipes(m_RBData, m_Settings),
	m_ModelShoppingListItems(m_RBData, m_Converter),
	m_ModelGoShopping(m_RBData, m_Converter),
	m_DlgInterface(),
	m_ShoppingListExporter(m_RBData, m_Settings, m_Converter, m_DlgInterface),
	m_Engine(),
	m_SaveLock()
{
	m_DlgInterface.setQmlEngine(m_Engine);

	QFile fileIn(m_Settings.applicationRecipeBookSaveFile());
	if(fileIn.exists())
	{
		QSharedPointer<IRBReader> spReader = SerializerFactory::getReader(FileFormat::Json);
		RBMetaData metaData;

		recipebook::RBDataWriteHandle handle(m_RBData, true);
		spReader->serialize(fileIn, metaData, handle.data());
	}

	m_ModelSortOrder.setSourceModel(&m_ModelCategories);
	m_AlternativesTypes.setSourceModel(&m_AlternativesGroups);
	m_FilterModelIngredients.setSourceModel(&m_ModelIngredients);
	m_FilterModelRecipes.setSourceModel(&m_ModelRecipes);
	m_FilterModelRecipeItems.setSourceModel(&m_ModelRecipeItems);

	connect(&m_ModelCategories, SIGNAL(categoryRenamed(quint32)),
			&m_ModelIngredients, SLOT(onCategoryRenamed(quint32)));
	connect(&m_ModelSortOrders, SIGNAL(sortOrderRenamed(quint32)),
			&m_ModelIngredients, SLOT(onSortOrderRenamed(quint32)));

	connect(&m_ModelIngredients, SIGNAL(ingredientRenamed(quint32)),
			&m_ModelRecipeItems, SLOT(onDependentItemChanged(quint32)));
	connect(&m_AlternativesGroups, SIGNAL(alternativesGroupChanged(quint32)),
			&m_ModelRecipeItems, SLOT(onDependentItemChanged(quint32)));

	connect(&m_ModelIngredients, SIGNAL(ingredientRenamed(quint32)),
			&m_ModelShoppingListItems, SLOT(onDependentItemChanged(quint32)));
	connect(&m_AlternativesGroups, SIGNAL(alternativesGroupChanged(quint32)),
			&m_ModelShoppingListItems, SLOT(onDependentItemChanged(quint32)));
	connect(&m_ModelShoppingRecipes, SIGNAL(recipeScalingChanged(quint32)),
			&m_ModelShoppingListItems, SLOT(onDependentItemChanged(quint32)));

	connect(&m_ModelShoppingListItems, SIGNAL(shoppingitemEnabledChanged(int)),
			&m_ModelShoppingRecipes, SLOT(onItemEnabledChanged(int)));

	connect(&m_Settings, SIGNAL(resetAllData()),
			this, SLOT(slotResetData()));
	connect(&m_Settings, SIGNAL(loadDefaultData()),
			this, SLOT(slotLoadDefaultData()));
	
	// onResetData
	connect(this, SIGNAL(signalDataReset()),
			&m_ModelSortOrder, SLOT(onDataReset()));
	connect(this, SIGNAL(signalDataReset()),
			&m_ModelCategories, SLOT(onDataReset()));
	connect(this, SIGNAL(signalDataReset()),
			&m_ModelSortOrders, SLOT(onDataReset()));
	connect(this, SIGNAL(signalDataReset()),
			&m_ModelIngredients, SLOT(onDataReset()));
	connect(this, SIGNAL(signalDataReset()),
			&m_AlternativesGroups, SLOT(onDataReset()));
	connect(this, SIGNAL(signalDataReset()),
			&m_ModelRecipes, SLOT(onDataReset()));
	connect(this, SIGNAL(signalDataReset()),
			&m_ModelShoppingRecipes, SLOT(onDataReset()));
	connect(this, SIGNAL(signalDataReset()),
			&m_ModelGoShopping, SLOT(onDataReset()));

	// Enable periodic saving routine
	QTimer *pTimer = new QTimer(this);
	connect(pTimer, SIGNAL(timeout()), this, SLOT(slotSave()));
	pTimer->start(c_SaveIntervalSeconds * 1000);
}

void RecipeBookUIContext::slotSave()
{
	if(!m_SaveLock.testAndSetAcquire(0, 1))
	{
		// Another thread is already saving, so we don't need to.
		return;
	}

	if(!QDir(m_Settings.applicationRecipeBookAppsDataFolder()).mkpath("."))
	{
		qCritical("Couldn't create path to recipe book file (%s)", qUtf8Printable(m_Settings.applicationRecipeBookAppsDataFolder()));
		m_SaveLock = 0;
		return;
	}

	if(!m_RBData.hasDataChanged())
	{
		m_SaveLock = 0;
		return;
	}

	QSharedPointer<IRBWriter> spWriter = SerializerFactory::getWriter(FileFormat::Json, m_Settings.getApplicationInstanceUID());
	QFile fileOut(m_Settings.applicationRecipeBookSaveFile());

	recipebook::RBDataReadHandle handle(m_RBData);
	if(!spWriter->serialize(handle.data(), fileOut))
	{
		qCritical("Couldn't write recipe book file \"%s\"", qUtf8Printable(m_Settings.applicationRecipeBookAppsDataFolder()));
	}

	m_RBData.setDataUnchanged();
	m_SaveLock = 0;
}

void RecipeBookUIContext::slotExport(QString strFileURL)
{
	QString localFileName = QUrl(strFileURL).toLocalFile();

	QFileInfo fi(localFileName);
	m_Settings.setLastUsedExportFolder(fi.absolutePath());

	QSharedPointer<IRBWriter> spWriter = SerializerFactory::getWriter(FileFormat::Json, m_Settings.getApplicationInstanceUID());
	QFile fileOut(localFileName);

	recipebook::RBDataReadHandle handle(m_RBData);
	if(!spWriter->serialize(handle.data(), fileOut))
	{
		qCritical("Couldn't export to file \"%s\"", qUtf8Printable(localFileName));
	}
}

void RecipeBookUIContext::slotImport(QString strFileURL)
{
	QString localFileName = QUrl(strFileURL).toLocalFile();

	QFileInfo fi(localFileName);
	m_Settings.setLastUsedImportFolder(fi.absolutePath());

	QSharedPointer<IRBReader> spReader = SerializerFactory::getReader(FileFormat::Json);
	RBMetaData metaData;
	RecipeBook recipeBook;
	QFile fileIn(localFileName);
	if(!spReader->serialize(fileIn, metaData, recipeBook))
	{
		qCritical("Couldn't import file \"%s\"", qUtf8Printable(localFileName));
	}
	else
	{
		{
			recipebook::RBDataWriteHandle handle(m_RBData);
			handle.data() = recipeBook;
		}
		emit signalDataReset();
	}
}

void RecipeBookUIContext::slotLoadDefaultData()
{
	QSharedPointer<IRBReader> spReader = SerializerFactory::getReader(FileFormat::Json);
	RBMetaData metaData;
	RecipeBook recipeBook;
	QFile fileIn(":/files/DefaultRecipeBook.json");
	if(!spReader->serialize(fileIn, metaData, recipeBook))
	{
		qCritical("Couldn't load default data");
	}
	else
	{
		{
			recipebook::RBDataWriteHandle handle(m_RBData);
			handle.data() = recipeBook;
		}
		emit signalDataReset();
	}
}

void RecipeBookUIContext::slotResetData()
{
	{
		recipebook::RBDataWriteHandle handle(m_RBData);
		handle.data().clearData();
	}

	emit signalDataReset();
}

bool RecipeBookUIContext::setupQml()
{
	QQmlContext* ctxt = m_Engine.rootContext();
	if(!setupNameLists(ctxt))
	{
		return false;
	}

	m_Engine.load(QUrl(QStringLiteral("qrc:/recipebook.qml")));
	if(m_Engine.rootObjects().isEmpty())
	{
		return false;
	}

	QObject* rootObject = m_Engine.rootObjects().first();
	if(!setupConnections(rootObject))
	{
		return false;
	}

	return true;
}

bool RecipeBookUIContext::setupNameLists(QQmlContext* context)
{
	if(context == nullptr)
	{
		return false;
	}

	context->setContextProperty("recipeBookSettings", &m_Settings);

	context->setContextProperty("shoppingListExporter", &m_ShoppingListExporter);

	context->setContextProperty("unitNames", m_Converter.getAllUnitNames());
	context->setContextProperty("unitNamesShort", m_Converter.getAllUnitShortNames());
    
	context->setContextProperty("sizeNames", m_Converter.getAllSizeNames());
	context->setContextProperty("statusNames", m_Converter.getAllStatusNames());

	context->setContextProperty("shoppingListOrderingNames", m_Converter.getAllShoppingListOrderingNames());

	context->setContextProperty("modelCategories", &m_ModelCategories);
	context->setContextProperty("modelSortOrder", &m_ModelSortOrder);

	context->setContextProperty("modelSortOrders", &m_ModelSortOrders);

	context->setContextProperty("modelIngredients", &m_ModelIngredients);
	context->setContextProperty("filterModelIngredients", &m_FilterModelIngredients);

	context->setContextProperty("alternativesGroups", &m_AlternativesGroups);
	context->setContextProperty("alternativesTypes", &m_AlternativesTypes);

	context->setContextProperty("modelRecipes", &m_ModelRecipes);
	context->setContextProperty("filterModelRecipes", &m_FilterModelRecipes);

	context->setContextProperty("modelRecipeItems", &m_ModelRecipeItems);
	context->setContextProperty("filterModelRecipeItems", &m_FilterModelRecipeItems);

	context->setContextProperty("modelShoppingRecipes", &m_ModelShoppingRecipes);
	context->setContextProperty("modelShoppingListItems", &m_ModelShoppingListItems);

	context->setContextProperty("modelGoShopping", &m_ModelGoShopping);

	return true;
}

bool RecipeBookUIContext::setupConnections(QObject* pRoot)
{
	QObject* fileDialogExport = pRoot->findChild<QObject*>(QStringLiteral("fileDialogExport"));
	QObject::connect(fileDialogExport, SIGNAL(onExport(QString)),
					 this, SLOT(slotExport(QString)));

	QObject* fileDialogImport = pRoot->findChild<QObject*>(QStringLiteral("fileDialogImport"));
	QObject::connect(fileDialogImport, SIGNAL(onImport(QString)),
					 this, SLOT(slotImport(QString)));

	QObject::connect(pRoot, SIGNAL(onClosingRecipeBook()),
					 this, SLOT(slotSave()));
	return true;
}
