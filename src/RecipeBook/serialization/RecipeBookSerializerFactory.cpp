#include "RecipeBookSerializerFactory.h"
#include "IRecipeBookSerializer.h"
#include "json/RecipeBookJsonWriter.h"
#include "json/RecipeBookJsonReader.h"

using namespace recipebook::serialization;

QSharedPointer<IRBWriter> SerializerFactory::getWriter(FileFormat format, QString strUID)
{
	switch(format)
	{
		case FileFormat::Json:
			return QSharedPointer<json::JsonWriter>::create(strUID, false);

		case FileFormat::JsonForApp:
			return QSharedPointer<json::JsonWriter>::create(strUID, true);

		default:
			return nullptr;
	}
}

QSharedPointer<IRBReader> SerializerFactory::getReader(FileFormat format)
{
	switch(format)
	{
		case FileFormat::Json:
		case FileFormat::JsonForApp:
			return QSharedPointer<json::JsonReader>::create();

		default:
			return nullptr;
	}
}
