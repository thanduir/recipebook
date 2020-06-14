#include "RBUnitTests.h"
#include "serialization/RecipeBookSerializerFactory.h"
#include "data/RecipeBook.h"

constexpr char* c_strFileInput					= "test.json";

constexpr char* c_strFileReference				= "reference.json";

constexpr char* c_strFileInputConfigs			= "test_rbconfigs.json";
constexpr char* c_strFileReferenceConfigs		= "reference_rbconfigs.json";

constexpr char* c_strFileOutput1				= "z_output_1.json";
constexpr char* c_strFileOutput2				= "z_output_2.json";
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
	fileTest1.open(QIODevice::ReadOnly | QIODevice::Text);
	QString strTest1 = fileTest1.readAll();
	fileTest1.close();
	
	QFile fileTest2(m_spTestDir->filePath(c_strFileOutput2));
	fileTest2.open(QIODevice::ReadOnly | QIODevice::Text);
	QString strTest2 = fileTest2.readAll();
	fileTest1.close();

	QFile fileRef(m_spTestDir->filePath(c_strFileReference));
	fileRef.open(QIODevice::ReadOnly | QIODevice::Text);
	QString strRef = fileRef.readAll();
	fileRef.close();

	QVERIFY(strTest1 == strRef);
	QVERIFY(strTest1 == strTest2);
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

	// Verify output

	QFile fileTest(m_spTestDir->filePath(c_strFileOutputConfigs));
	fileTest.open(QIODevice::ReadOnly | QIODevice::Text);
	QString strTest = fileTest.readAll();
	fileTest.close();

	QFile fileRef(m_spTestDir->filePath(c_strFileReferenceConfigs));
	fileRef.open(QIODevice::ReadOnly | QIODevice::Text);
	QString strRef = fileRef.readAll();
	fileRef.close();

	QVERIFY(strTest == strRef);
}
