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
		
		virtual void enableAlternativesGroupsSorting() = 0;
		virtual void disableAlternativesGroupsSorting() = 0;
	};
}

#endif
