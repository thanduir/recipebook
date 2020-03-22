#include "Recipes/serialization/RecipeBookSerializerFactory.h"
#include "Recipes/data/RecipeBook.h"
#include "UI\recipebookwindow.h"
#include <QtWidgets/QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
	// TODO(phiwid): Temporär datei lesen und wieder schreiben, dann vergleichen! (-> Kann ich das unittest-mässig machen?)
	QSharedPointer<recipebook::serialization::IRBReader> spReader = recipebook::serialization::SerializerFactory::getReader(recipebook::serialization::FileFormat::Json);
	recipebook::serialization::RBMetaData metaData;
	recipebook::RecipeBook recipeBook;
	QFile fileIn("E:\\programming\\RecipeBook\\Examples\\test.json");
	if(!spReader->serialize(fileIn, metaData, recipeBook))
	{
		return 1;
	}

	QSharedPointer<recipebook::serialization::IRBWriter> spWriter = recipebook::serialization::SerializerFactory::getWriter(recipebook::serialization::FileFormat::Json, metaData.strUID);
	QFile fileOut("E:\\programming\\RecipeBook\\Examples\\z_output.json");
	if(!spWriter->serialize(recipeBook, fileOut))
	{
		return 1;
	}

	recipebook::serialization::RBMetaData metaData2;
	recipebook::RecipeBook recipeBook2;
	QFile fileIn2("E:\\programming\\RecipeBook\\Examples\\z_output.json");
	if(!spReader->serialize(fileIn2, metaData2, recipeBook2))
	{
		return 1;
	}

	QFile fileOut2("E:\\programming\\RecipeBook\\Examples\\z_output2.json");
	if(!spWriter->serialize(recipeBook2, fileOut2))
	{
		return 1;
	}

	return 0;

	/*QApplication a(argc, argv);
	RecipeBookWindow w;
	w.show();
	return a.exec();*/
}
