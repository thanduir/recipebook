#include "RecipeBookDataHandler.h"
#include <QDir>
#include <QTimer>
#include <QUrl>
#include <QFileInfo>
#include "serialization/RecipeBookSerializerFactory.h"
#include "data/Size.h"
#include "data/Status.h"
#include "data/Unit.h"

using namespace recipebook::UI;
using namespace recipebook::serialization;

const int c_SaveIntervalSeconds = 300;

RecipeBookDataHandler::RecipeBookDataHandler()
:	m_RecipeBook(),
	m_Converter(),
	m_Settings(),
	m_ModelCategories(m_RecipeBook),
	m_ModelSortOrder(m_RecipeBook),
	m_ModelProvenance(m_RecipeBook, m_Converter),
	m_ModelSortOrders(),
	m_ModelIngredients(m_RecipeBook, m_Settings, m_Converter),
	m_FilterModelIngredients(),
	m_AlternativesGroups(m_RecipeBook, m_Converter),
	m_AlternativesTypes(),
	m_ModelRecipes(m_RecipeBook, m_Settings),
	m_FilterModelRecipes(),
	m_ModelRecipeItems(m_RecipeBook, m_Converter)
{
	QFile fileIn(m_Settings.applicationRecipeBookSaveFile());
	if(fileIn.exists())
	{
		QSharedPointer<IRBReader> spReader = SerializerFactory::getReader(FileFormat::Json);
		RBMetaData metaData;
		spReader->serialize(fileIn, metaData, m_RecipeBook);
	}

	m_ModelSortOrder.setSourceModel(&m_ModelCategories);
	m_ModelSortOrders.setSourceModel(&m_ModelProvenance);
	m_AlternativesTypes.setSourceModel(&m_AlternativesGroups);
	m_FilterModelIngredients.setSourceModel(&m_ModelIngredients);
	m_FilterModelRecipes.setSourceModel(&m_ModelRecipes);

	connect(&m_ModelCategories, SIGNAL(categoryRenamed(quint32)),
			&m_ModelIngredients, SLOT(onCategoryRenamed(quint32)));
	connect(&m_ModelProvenance, SIGNAL(provenanceRenamed(quint32)),
			&m_ModelIngredients, SLOT(onSortOrderRenamed(quint32)));
	connect(&m_ModelIngredients, SIGNAL(ingredientRenamed(quint32)),
			&m_ModelRecipeItems, SLOT(onDependentItemChanged(quint32)));
	connect(&m_AlternativesGroups, SIGNAL(alternativesGroupChanged(quint32)),
			&m_ModelRecipeItems, SLOT(onDependentItemChanged(quint32)));

	connect(&m_Settings, SIGNAL(resetAllData()),
			this, SLOT(slotResetData()));

	// onResetData
	connect(this, SIGNAL(signalDataReset()),
			&m_ModelSortOrder, SLOT(onDataReset()));
	connect(this, SIGNAL(signalDataReset()),
			&m_ModelCategories, SLOT(onDataReset()));
	connect(this, SIGNAL(signalDataReset()),
			&m_ModelProvenance, SLOT(onDataReset()));
	connect(this, SIGNAL(signalDataReset()),
			&m_ModelIngredients, SLOT(onDataReset()));
	connect(this, SIGNAL(signalDataReset()),
			&m_AlternativesGroups, SLOT(onDataReset()));
	connect(this, SIGNAL(signalDataReset()),
			&m_ModelRecipes, SLOT(onDataReset()));
	connect(this, SIGNAL(signalDataReset()),
			&m_ModelRecipeItems, SLOT(onDataReset()));

	// Enable periodic saving routine
	// TODO: Should this be configurable? (automatic save on/off? should the interval also be setable?)
	//		-> Yes, make both configurable (so that the user can decide how much safety he needs)!
	/*QTimer *pTimer = new QTimer(this);
	connect(pTimer, SIGNAL(timeout()), this, SLOT(slotSave()));
	pTimer->start(c_SaveIntervalSeconds * 1000);*/
}

void RecipeBookDataHandler::slotSave()
{
	if(!QDir(m_Settings.applicationRecipeBookAppsDataFolder()).mkpath("."))
	{
		// TODO: Report error?
		return;
	}

	QString localFileName = m_Settings.applicationRecipeBookSaveFile();
				
	QSharedPointer<IRBWriter> spWriter = SerializerFactory::getWriter(FileFormat::Json, m_Settings.getApplicationInstanceUID());
	QFile fileOut(localFileName);

	// TODO: Thread safety! (How to ensure that the data isn't changed in this period?)
	//		-> Ensure no concurrent changes happen and make sure this method isn't called twice concurrently!
	//		=> MAKE SURE THIS IS RESPECTED IN ALL OPERATIONS THAT CHANGE RecipeBook!
	//		=> I might need to add a separation between the RecipeBook-objects and access to it by adding some kind of handle object in between 
	//			(and keeping of pointers might not be allowed anymore)!
	if(!spWriter->serialize(m_RecipeBook, fileOut))
	{
		// TODO: Report error?
	}
}

void RecipeBookDataHandler::slotExport(QString strFileURL)
{
	QString localFileName = QUrl(strFileURL).toLocalFile();

	QFileInfo fi(localFileName);
	m_Settings.setLastUsedExportFolder(fi.absolutePath());

	QSharedPointer<IRBWriter> spWriter = SerializerFactory::getWriter(FileFormat::Json, m_Settings.getApplicationInstanceUID());
	QFile fileOut(localFileName);
	if(!spWriter->serialize(m_RecipeBook, fileOut))
	{
		// TODO: Error message!
	}
}

void RecipeBookDataHandler::slotImport(QString strFileURL)
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
		// TODO: Error message!
	}
	else
	{
		m_RecipeBook = recipeBook;
		emit signalDataReset();
	}
}

void RecipeBookDataHandler::slotResetData()
{
	m_RecipeBook.clearData();
	emit signalDataReset();
}

recipebook::RecipeBookSettings& RecipeBookDataHandler::getRecipeBookSettings()
{
	return m_Settings;
}

QStringList RecipeBookDataHandler::getAllUnitNames() const
{
	return m_Converter.getAllUnitNames();
}

QStringList RecipeBookDataHandler::getAllUnitShortNames() const
{
	return m_Converter.getAllUnitShortNames();
}

QStringList RecipeBookDataHandler::getAllSizeNames() const
{
	return m_Converter.getAllSizeNames();
}

QStringList RecipeBookDataHandler::getAllStatusNames() const
{ 
	return m_Converter.getAllStatusNames();
}

ListModelCategories& RecipeBookDataHandler::getCategoriesModel()
{
	return m_ModelCategories;
}

SortModelSortOrder& RecipeBookDataHandler::getSortOrderModel()
{
	return m_ModelSortOrder;
}

FilterModelSortOrders& RecipeBookDataHandler::getSortOrdersModel()
{
	return m_ModelSortOrders;
}

ListModelProvenance& RecipeBookDataHandler::getProvenanceModel()
{
	return m_ModelProvenance;
}

ListModelIngredients& RecipeBookDataHandler::getIngredientsModel()
{
	return m_ModelIngredients;
}

FilterModelIngredients& RecipeBookDataHandler::getIngredientsFilterModel()
{
	return m_FilterModelIngredients;
}

ListModelAlternativesGroups& RecipeBookDataHandler::getAlternativesGroups()
{
	return m_AlternativesGroups;
}

FilterModelAlternativesTypes& RecipeBookDataHandler::getAlternativesTypes()
{
	return m_AlternativesTypes;
}

ListModelRecipes& RecipeBookDataHandler::getRecipesModel()
{
	return m_ModelRecipes;
}

FilterModelRecipes& RecipeBookDataHandler::getRecipesFilterModel()
{
	return m_FilterModelRecipes;
}

ListModelRecipeItems& RecipeBookDataHandler::getRecipeItemsModel()
{
	return m_ModelRecipeItems;
}
