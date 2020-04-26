#include "RecipeBookUIData.h"
#include <QtGlobal>
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

RecipeBookUIData::RecipeBookUIData()
:	m_RBData(),
	m_Converter(),
	m_Settings(),
	m_ModelCategories(m_RBData),
	m_ModelSortOrder(m_RBData),
	m_ModelProvenance(m_RBData, m_Converter),
	m_ModelSortOrders(),
	m_ModelIngredients(m_RBData, m_Settings, m_Converter),
	m_FilterModelIngredients(),
	m_AlternativesGroups(m_RBData, m_Converter),
	m_AlternativesTypes(),
	m_ModelRecipes(m_RBData, m_Settings),
	m_FilterModelRecipes(),
	m_ModelRecipeItems(m_RBData, m_Converter),
	m_SaveLock()
{
	QFile fileIn(m_Settings.applicationRecipeBookSaveFile());
	if(fileIn.exists())
	{
		QSharedPointer<IRBReader> spReader = SerializerFactory::getReader(FileFormat::Json);
		RBMetaData metaData;

		recipebook::RBDataWriteHandle handle(m_RBData);
		spReader->serialize(fileIn, metaData, handle.data());
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
	QTimer *pTimer = new QTimer(this);
	connect(pTimer, SIGNAL(timeout()), this, SLOT(slotSave()));
	pTimer->start(c_SaveIntervalSeconds * 1000);
}

void RecipeBookUIData::slotSave()
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

	QString localFileName = m_Settings.applicationRecipeBookSaveFile();
				
	QSharedPointer<IRBWriter> spWriter = SerializerFactory::getWriter(FileFormat::Json, m_Settings.getApplicationInstanceUID());
	QFile fileOut(localFileName);

	recipebook::RBDataReadHandle handle(m_RBData);
	if(!spWriter->serialize(handle.data(), fileOut))
	{
		qCritical("Couldn't write recipe book file \"%s\"", qUtf8Printable(m_Settings.applicationRecipeBookAppsDataFolder()));
	}

	m_SaveLock = 0;
}

void RecipeBookUIData::slotExport(QString strFileURL)
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

void RecipeBookUIData::slotImport(QString strFileURL)
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
		recipebook::RBDataWriteHandle handle(m_RBData);
		handle.data() = recipeBook;
		emit signalDataReset();
	}
}

void RecipeBookUIData::slotResetData()
{
	{
		recipebook::RBDataWriteHandle handle(m_RBData);
		handle.data().clearData();
	}

	emit signalDataReset();
}

recipebook::RecipeBookSettings& RecipeBookUIData::getRecipeBookSettings()
{
	return m_Settings;
}

QStringList RecipeBookUIData::getAllUnitNames() const
{
	return m_Converter.getAllUnitNames();
}

QStringList RecipeBookUIData::getAllUnitShortNames() const
{
	return m_Converter.getAllUnitShortNames();
}

QStringList RecipeBookUIData::getAllSizeNames() const
{
	return m_Converter.getAllSizeNames();
}

QStringList RecipeBookUIData::getAllStatusNames() const
{ 
	return m_Converter.getAllStatusNames();
}

ListModelCategories& RecipeBookUIData::getCategoriesModel()
{
	return m_ModelCategories;
}

SortModelSortOrder& RecipeBookUIData::getSortOrderModel()
{
	return m_ModelSortOrder;
}

FilterModelSortOrders& RecipeBookUIData::getSortOrdersModel()
{
	return m_ModelSortOrders;
}

ListModelProvenance& RecipeBookUIData::getProvenanceModel()
{
	return m_ModelProvenance;
}

ListModelIngredients& RecipeBookUIData::getIngredientsModel()
{
	return m_ModelIngredients;
}

FilterModelIngredients& RecipeBookUIData::getIngredientsFilterModel()
{
	return m_FilterModelIngredients;
}

ListModelAlternativesGroups& RecipeBookUIData::getAlternativesGroups()
{
	return m_AlternativesGroups;
}

FilterModelAlternativesTypes& RecipeBookUIData::getAlternativesTypes()
{
	return m_AlternativesTypes;
}

ListModelRecipes& RecipeBookUIData::getRecipesModel()
{
	return m_ModelRecipes;
}

FilterModelRecipes& RecipeBookUIData::getRecipesFilterModel()
{
	return m_FilterModelRecipes;
}

ListModelRecipeItems& RecipeBookUIData::getRecipeItemsModel()
{
	return m_ModelRecipeItems;
}
