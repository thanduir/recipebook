#include "RecipeBookDataHandler.h"
#include <QUrl>
#include "serialization/RecipeBookSerializerFactory.h"
#include "data/Size.h"
#include "data/Status.h"
#include "data/Unit.h"

using namespace recipebook::UI;
using namespace recipebook::serialization;

// TODO: Replace with non-temp. file!
constexpr char* c_strFileInput		= "..\\..\\UnitTestData\\SerializeTest\\test.json";
// TODO: Replace with real UID!
constexpr char* c_strUID			= "AAAABBBBCCCC";

RecipeBookDataHandler::RecipeBookDataHandler()
:	m_RecipeBook(),
	m_Converter(),
	m_ModelCategories(m_RecipeBook),
	m_ModelSortOrder(m_RecipeBook),
	m_ModelProvenance(m_RecipeBook, m_Converter),
	m_ModelSortOrders(),
	m_ModelIngredients(m_RecipeBook, m_Converter),
	m_FilterModelIngredients(),
	m_AlternativesGroups(m_RecipeBook, m_Converter),
	m_AlternativesTypes(),
	m_ModelRecipes(m_RecipeBook),
	m_FilterModelRecipes(),
	m_ModelRecipeItems(m_RecipeBook, m_Converter)
{
	QSharedPointer<IRBReader> spReader = SerializerFactory::getReader(FileFormat::Json);
	RBMetaData metaData;
	QFile fileIn(c_strFileInput);
	spReader->serialize(fileIn, metaData, m_RecipeBook);

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
}

void RecipeBookDataHandler::slotSaveAs(QString strFileURL)
{
	QString localFileName = QUrl(strFileURL).toLocalFile();

	QSharedPointer<IRBWriter> spWriter = SerializerFactory::getWriter(FileFormat::Json, c_strUID);
	RBMetaData metaData;
	QFile fileOut(localFileName);
	spWriter->serialize(m_RecipeBook, fileOut);
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
