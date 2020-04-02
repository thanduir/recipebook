#include "RecipeBookDataHandler.h"
#include <QUrl>
#include "serialization/RecipeBookSerializerFactory.h"
#include "data/Size.h"
#include "data/Status.h"
#include "data/Unit.h"

using namespace recipebook::UI;
using namespace recipebook::serialization;

// TODO(phiwid): Replace with non-temp. file!
constexpr char* c_strFileInput		= "..\\..\\UnitTestData\\SerializeTest\\test.json";
// TODO(phiwid): Replace with real UID!
constexpr char* c_strUID			= "AAAABBBBCCCC";

RecipeBookDataHandler::RecipeBookDataHandler()
	: m_RecipeBook(),
	m_Converter(),
	m_ModelCategories(m_RecipeBook),
	m_ModelSortOrders(m_RecipeBook),
	m_ModelProvenance(m_RecipeBook, m_Converter),
	m_ModelIngredients(m_RecipeBook, m_Converter)
{
	QSharedPointer<IRBReader> spReader = SerializerFactory::getReader(FileFormat::Json);
	RBMetaData metaData;
	QFile fileIn(c_strFileInput);
	spReader->serialize(fileIn, metaData, m_RecipeBook);
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

ListModelSortOrders& RecipeBookDataHandler::getSortOrdersModel()
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
