#ifndef RECIPEBOOK_SERIALIZATION_IRBSERIALIZER_H
#define RECIPEBOOK_SERIALIZATION_IRBSERIALIZER_H

#include <QFile>

namespace recipebook
{
	class RecipeBook;
}

namespace recipebook::serialization
{
	struct RBMetaData
	{
		QString strOrigin;
		QString strUID;
	};
    
	class IRBWriter
	{
	public:
		virtual bool serialize(const RecipeBook& rRecipeBook, QFile& file) = 0;
	};

	class IRBReader
	{
	public:
		virtual bool serialize(QFile& file, RBMetaData& rMetaData, RecipeBook& rRecipeBook) = 0;
	};

	/* TODO: Write concept similar to what's being done in the app! See below:

		Concept if the file already exists:
			* write to filename.new
			* delete filename.old if it already exists
			* move existing to filename.old
			* move filename.new to filename
	*/
}

#endif
