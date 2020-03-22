#ifndef RECIPEBOOK_SERIALIZATION_JSON_JSONREADER_H
#define RECIPEBOOK_SERIALIZATION_JSON_JSONREADER_H

#include "../IRecipeBookSerializer.h"

namespace recipebook
{
    class Ingredient;
    class Recipe;
    class RecipeItemGroup;
}

namespace recipebook::serialization::json
{
    class JsonReader final : public recipebook::serialization::IRBReader
    {
    public:
        virtual bool serialize(QFile& file, RBMetaData& rMetaData, RecipeBook& rRecipeBook) override final;

    private:
        JsonReader();
        
        friend class QSharedPointer<JsonReader>;
    };
}

#endif
