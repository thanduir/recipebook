#include "RBUnitTests.h"
#include "serialization/RecipeBookSerializerFactory.h"
#include "data/RecipeBook.h"

void RBUnitTests::serializeTest()
{
	QSharedPointer<recipebook::serialization::IRBReader> spReader = recipebook::serialization::SerializerFactory::getReader(recipebook::serialization::FileFormat::Json);
	recipebook::serialization::RBMetaData metaData;
	recipebook::RecipeBook recipeBook;
	QFile fileIn("E:\\programming\\RecipeBook\\Examples\\test.json");
	QVERIFY(spReader->serialize(fileIn, metaData, recipeBook));

	QSharedPointer<recipebook::serialization::IRBWriter> spWriter = recipebook::serialization::SerializerFactory::getWriter(recipebook::serialization::FileFormat::Json, metaData.strUID);
	QFile fileOut("E:\\programming\\RecipeBook\\Examples\\z_output.json");
	QVERIFY(spWriter->serialize(recipeBook, fileOut));

	recipebook::serialization::RBMetaData metaData2;
	recipebook::RecipeBook recipeBook2;
	QFile fileIn2("E:\\programming\\RecipeBook\\Examples\\z_output.json");
	QVERIFY(spReader->serialize(fileIn2, metaData2, recipeBook2));

	QFile fileOut2("E:\\programming\\RecipeBook\\Examples\\z_output2.json");
	QVERIFY(spWriter->serialize(recipeBook2, fileOut2));

	// TODO(phiwid): Verify output as well (not just that writing / reading works)!
}
