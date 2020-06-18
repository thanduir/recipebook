#include "RecipeBookJsonReader.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "RecipeBookJsonReaderV1.h"
#include "RecipeBookJsonReaderV2.h"

using namespace recipebook;
using namespace recipebook::serialization;

json::JsonReader::JsonReader()
{
}

bool json::JsonReader::serialize(QFile& file, RBMetaData& rMetaData, RecipeBook& rRecipeBook)
{
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
        qWarning("Couldn't open save file.");
        return false;
    }

	QByteArray saveData = file.readAll();
    file.close();

	QJsonDocument jsonDoc(QJsonDocument::fromJson(saveData));
    
    if(jsonDoc.isArray())
    {
        // Version 1
        QJsonArray rootArray = jsonDoc.array();
        json::JsonReaderV1 reader;
		return reader.read(rootArray, rMetaData, rRecipeBook);
    }
    else
    {
        // Version 2
        QJsonObject rootObject = jsonDoc.object();
        json::JsonReaderV2 reader;
		reader.setUseAlternativesGroupsSorting(m_bUseAlternativesGroupsSorting);
        return reader.read(rootObject, rMetaData, rRecipeBook);
    }
}
