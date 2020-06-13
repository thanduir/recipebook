#include "RBUnitTests.h"
#include "serialization/RecipeBookSerializerFactory.h"
#include "data/RecipeBook.h"

constexpr char* c_strFileInput					= "test.json";

constexpr char* c_strFileReference				= "reference.json";
constexpr char* c_strFileReferenceApp			= "reference_app.json";

constexpr char* c_strFileInputConfigs			= "test_rbconfigs.json";
constexpr char* c_strFileReferenceConfigs		= "reference_rbconfigs.json";
constexpr char* c_strFileReferenceConfigsApp	= "reference_rbconfigs_app.json";

// TODO: Where to save those?	
constexpr char* c_strFileOutput1				= "z_output_1.json";
constexpr char* c_strFileOutput2				= "z_output_2.json";
constexpr char* c_strFileOutputApp				= "z_output_app.json";
constexpr char* c_strFileOutputConfigsApp		= "z_output_rbconfigs_app.json";
constexpr char* c_strFileOutputConfigs			= "z_output_rbconfigs.json";

void RBUnitTests::initTestCase()
{
	m_spTestDir = QTest::qExtractTestData("serializeTest");

	qDebug(m_spTestDir->path().toLatin1());
}

void RBUnitTests::serializeTest()
{
	// Serialize test

	QSharedPointer<recipebook::serialization::IRBReader> spReader = recipebook::serialization::SerializerFactory::getReader(recipebook::serialization::FileFormat::Json);
	recipebook::serialization::RBMetaData metaData;
	recipebook::RecipeBook recipeBook;
	QFile fileIn(m_spTestDir->filePath(c_strFileInput));
	QVERIFY(spReader->serialize(fileIn, metaData, recipeBook));

	QSharedPointer<recipebook::serialization::IRBWriter> spWriter = recipebook::serialization::SerializerFactory::getWriter(recipebook::serialization::FileFormat::Json, metaData.strUID);
	QFile fileOut(m_spTestDir->filePath(c_strFileOutput1));
	QVERIFY(spWriter->serialize(recipeBook, fileOut));

	recipebook::serialization::RBMetaData metaData2;
	recipebook::RecipeBook recipeBook2;
	QFile fileIn2(m_spTestDir->filePath(c_strFileOutput1));
	QVERIFY(spReader->serialize(fileIn2, metaData2, recipeBook2));

	QFile fileOut2(m_spTestDir->filePath(c_strFileOutput2));
	QVERIFY(spWriter->serialize(recipeBook2, fileOut2));

	// Verify output

	QFile fileTest1(m_spTestDir->filePath(c_strFileOutput1));
	fileTest1.open(QIODevice::ReadOnly);
	QString strTest1 = fileTest1.readAll();
	fileTest1.close();
	
	QFile fileTest2(m_spTestDir->filePath(c_strFileOutput2));
	fileTest2.open(QIODevice::ReadOnly);
	QString strTest2 = fileTest2.readAll();
	fileTest1.close();

	QFile fileRef(m_spTestDir->filePath(c_strFileReference));
	fileRef.open(QIODevice::ReadOnly);
	QString strRef = fileRef.readAll();
	fileRef.close();

	QVERIFY(strTest1 == strRef);
	QVERIFY(strTest1 == strTest2);
}

void RBUnitTests::serializeTestApp()
{
	// Read base json (v1)
	QSharedPointer<recipebook::serialization::IRBReader> spReader = recipebook::serialization::SerializerFactory::getReader(recipebook::serialization::FileFormat::Json);
	recipebook::serialization::RBMetaData metaData;
	recipebook::RecipeBook recipeBook;
	QFile fileIn(m_spTestDir->filePath(c_strFileInput));
	QVERIFY(spReader->serialize(fileIn, metaData, recipeBook));

	// Write file JsonForApp
	QSharedPointer<recipebook::serialization::IRBWriter> spWriter = recipebook::serialization::SerializerFactory::getWriter(recipebook::serialization::FileFormat::JsonForApp, metaData.strUID);
	QFile fileOut(m_spTestDir->filePath(c_strFileOutputApp));
	QVERIFY(spWriter->serialize(recipeBook, fileOut));

	// Test read JsonForApp
	recipebook::serialization::RBMetaData metaData2;
	recipebook::RecipeBook recipeBook2;
	QFile fileIn2(m_spTestDir->filePath(c_strFileOutputApp));
	QVERIFY(spReader->serialize(fileIn2, metaData2, recipeBook2));

	// Verify output

	QFile fileTest(m_spTestDir->filePath(c_strFileOutputApp));
	fileTest.open(QIODevice::ReadOnly);
	QString strTest = fileTest.readAll();
	fileTest.close();
	
	QFile fileRef(m_spTestDir->filePath(c_strFileReferenceApp));
	fileRef.open(QIODevice::ReadOnly);
	QString strRef = fileRef.readAll();
	fileRef.close();

	QVERIFY(strTest == strRef);
}

void RBUnitTests::serializeTestRBConfigs()
{
	// Serialize test

	QSharedPointer<recipebook::serialization::IRBReader> spReader = recipebook::serialization::SerializerFactory::getReader(recipebook::serialization::FileFormat::Json);
	recipebook::serialization::RBMetaData metaData;
	recipebook::RecipeBook recipeBook;
	QFile fileIn(m_spTestDir->filePath(c_strFileInputConfigs));
	QVERIFY(spReader->serialize(fileIn, metaData, recipeBook));

	QSharedPointer<recipebook::serialization::IRBWriter> spWriter = recipebook::serialization::SerializerFactory::getWriter(recipebook::serialization::FileFormat::Json, metaData.strUID);
	QFile fileOut(m_spTestDir->filePath(c_strFileOutputConfigs));
	QVERIFY(spWriter->serialize(recipeBook, fileOut));

	// Write file JsonForApp

	QSharedPointer<recipebook::serialization::IRBWriter> spWriterApp = recipebook::serialization::SerializerFactory::getWriter(recipebook::serialization::FileFormat::JsonForApp, metaData.strUID);
	QFile fileOutApp(m_spTestDir->filePath(c_strFileOutputConfigsApp));
	QVERIFY(spWriterApp->serialize(recipeBook, fileOutApp));

	// Verify output

	QFile fileTest(m_spTestDir->filePath(c_strFileOutputConfigs));
	fileTest.open(QIODevice::ReadOnly);
	QString strTest = fileTest.readAll();
	fileTest.close();

	QFile fileRef(m_spTestDir->filePath(c_strFileReferenceConfigs));
	fileRef.open(QIODevice::ReadOnly);
	QString strRef = fileRef.readAll();
	fileRef.close();

	QVERIFY(strTest == strRef);

	// Verify output for apps

	QFile fileTestApp(m_spTestDir->filePath(c_strFileOutputConfigsApp));
	fileTestApp.open(QIODevice::ReadOnly);
	QString strTestApp = fileTestApp.readAll();
	fileTestApp.close();

	QFile fileRefApp(m_spTestDir->filePath(c_strFileReferenceConfigsApp));
	fileRefApp.open(QIODevice::ReadOnly);
	QString strRefApp = fileRefApp.readAll();
	fileRefApp.close();

	QVERIFY(strTestApp == strRefApp);
}
