#include "RBUnitTests.h"
#include "serialization/RecipeBookSerializerFactory.h"
#include "data/RecipeBook.h"

constexpr char* strFileInput			= "UnitTestData\\SerializeTest\\test.json";

constexpr char* strFileReference		= "UnitTestData\\SerializeTest\\reference.json";
constexpr char* strFileOutput1			= "UnitTestData\\SerializeTest\\z_output_1.json";
constexpr char* strFileOutput2			= "UnitTestData\\SerializeTest\\z_output_2.json";

constexpr char* strFileReferenceApp		= "UnitTestData\\SerializeTest\\reference_app.json";
constexpr char* strFileOutputApp		= "UnitTestData\\SerializeTest\\z_output_app.json";

constexpr char* strFileInputConfigs			= "UnitTestData\\SerializeTest\\test_rbconfigs.json";
constexpr char* strFileReferenceConfigs		= "UnitTestData\\SerializeTest\\reference_rbconfigs.json";
constexpr char* strFileOutputConfigs		= "UnitTestData\\SerializeTest\\z_output_rbconfigs.json";
constexpr char* strFileReferenceConfigsApp	= "UnitTestData\\SerializeTest\\reference_rbconfigs_app.json";
constexpr char* strFileOutputConfigsApp		= "UnitTestData\\SerializeTest\\z_output_rbconfigs_app.json";

void RBUnitTests::serializeTest()
{
	// Serialize test

	QSharedPointer<recipebook::serialization::IRBReader> spReader = recipebook::serialization::SerializerFactory::getReader(recipebook::serialization::FileFormat::Json);
	recipebook::serialization::RBMetaData metaData;
	recipebook::RecipeBook recipeBook;
	QFile fileIn(strFileInput);
	QVERIFY(spReader->serialize(fileIn, metaData, recipeBook));

	QSharedPointer<recipebook::serialization::IRBWriter> spWriter = recipebook::serialization::SerializerFactory::getWriter(recipebook::serialization::FileFormat::Json, metaData.strUID);
	QFile fileOut(strFileOutput1);
	QVERIFY(spWriter->serialize(recipeBook, fileOut));

	recipebook::serialization::RBMetaData metaData2;
	recipebook::RecipeBook recipeBook2;
	QFile fileIn2(strFileOutput1);
	QVERIFY(spReader->serialize(fileIn2, metaData2, recipeBook2));

	QFile fileOut2(strFileOutput2);
	QVERIFY(spWriter->serialize(recipeBook2, fileOut2));

	// Verify output

	QFile fileTest1(strFileOutput1);
	fileTest1.open(QIODevice::ReadOnly);
	QString strTest1 = fileTest1.readAll();
	fileTest1.close();
	
	QFile fileTest2(strFileOutput2);
	fileTest2.open(QIODevice::ReadOnly);
	QString strTest2 = fileTest2.readAll();
	fileTest1.close();

	QFile fileRef(strFileReference);
	fileRef.open(QIODevice::ReadOnly);
	QString strRef = fileRef.readAll();
	fileRef.close();

	QVERIFY(strTest1 == strRef);
	QVERIFY(strTest1 == strTest2);

	// Remove files after successful test
	fileTest1.remove();
	fileTest2.remove();
}

void RBUnitTests::serializeTestApp()
{
	// Read base json (v1)
	QSharedPointer<recipebook::serialization::IRBReader> spReader = recipebook::serialization::SerializerFactory::getReader(recipebook::serialization::FileFormat::Json);
	recipebook::serialization::RBMetaData metaData;
	recipebook::RecipeBook recipeBook;
	QFile fileIn(strFileInput);
	QVERIFY(spReader->serialize(fileIn, metaData, recipeBook));

	// Write file JsonForApp
	QSharedPointer<recipebook::serialization::IRBWriter> spWriter = recipebook::serialization::SerializerFactory::getWriter(recipebook::serialization::FileFormat::JsonForApp, metaData.strUID);
	QFile fileOut(strFileOutputApp);
	QVERIFY(spWriter->serialize(recipeBook, fileOut));

	// Test read JsonForApp
	recipebook::serialization::RBMetaData metaData2;
	recipebook::RecipeBook recipeBook2;
	QFile fileIn2(strFileOutputApp);
	QVERIFY(spReader->serialize(fileIn2, metaData2, recipeBook2));

	// Verify output

	QFile fileTest(strFileOutputApp);
	fileTest.open(QIODevice::ReadOnly);
	QString strTest = fileTest.readAll();
	fileTest.close();
	
	QFile fileRef(strFileReferenceApp);
	fileRef.open(QIODevice::ReadOnly);
	QString strRef = fileRef.readAll();
	fileRef.close();

	QVERIFY(strTest == strRef);

	// Remove file after successful test
	fileTest.remove();
}

void RBUnitTests::serializeTestRBConfigs()
{
	// Serialize test

	QSharedPointer<recipebook::serialization::IRBReader> spReader = recipebook::serialization::SerializerFactory::getReader(recipebook::serialization::FileFormat::Json);
	recipebook::serialization::RBMetaData metaData;
	recipebook::RecipeBook recipeBook;
	QFile fileIn(strFileInputConfigs);
	QVERIFY(spReader->serialize(fileIn, metaData, recipeBook));

	QSharedPointer<recipebook::serialization::IRBWriter> spWriter = recipebook::serialization::SerializerFactory::getWriter(recipebook::serialization::FileFormat::Json, metaData.strUID);
	QFile fileOut(strFileOutputConfigs);
	QVERIFY(spWriter->serialize(recipeBook, fileOut));

	// Write file JsonForApp

	QSharedPointer<recipebook::serialization::IRBWriter> spWriterApp = recipebook::serialization::SerializerFactory::getWriter(recipebook::serialization::FileFormat::JsonForApp, metaData.strUID);
	QFile fileOutApp(strFileOutputConfigsApp);
	QVERIFY(spWriterApp->serialize(recipeBook, fileOutApp));

	// Verify output

	QFile fileTest(strFileOutputConfigs);
	fileTest.open(QIODevice::ReadOnly);
	QString strTest = fileTest.readAll();
	fileTest.close();

	QFile fileRef(strFileReferenceConfigs);
	fileRef.open(QIODevice::ReadOnly);
	QString strRef = fileRef.readAll();
	fileRef.close();

	QVERIFY(strTest == strRef);

	// Remove files after successful test
	fileTest.remove();

	// Verify output for apps

	QFile fileTestApp(strFileOutputConfigsApp);
	fileTestApp.open(QIODevice::ReadOnly);
	QString strTestApp = fileTestApp.readAll();
	fileTestApp.close();

	QFile fileRefApp(strFileReferenceConfigsApp);
	fileRefApp.open(QIODevice::ReadOnly);
	QString strRefApp = fileRefApp.readAll();
	fileRefApp.close();

	QVERIFY(strTestApp == strRefApp);

	// Remove files after successful test
	fileTestApp.remove();
}
