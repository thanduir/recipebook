#include "serialization/RecipeBookSerializerFactory.h"
#include "serialization/IRecipeBookSerializer.h"
#include "json/RecipeBookJsonWriter.h"
#include "json/RecipeBookJsonReader.h"

using namespace recipebook::serialization;

QSharedPointer<IRBWriter> SerializerFactory::getWriter(FileFormat format, QString strUID)
{
	switch(format)
	{
		case FileFormat::Json:
			return QSharedPointer<json::JsonWriter>::create(strUID);

		default:
			return nullptr;
	}
}

QSharedPointer<IRBReader> SerializerFactory::getReader(FileFormat format)
{
	switch(format)
	{
		case FileFormat::Json:
			return QSharedPointer<json::JsonReader>::create();

		default:
			return nullptr;
	}
}
