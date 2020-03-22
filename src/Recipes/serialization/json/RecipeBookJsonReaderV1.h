#ifndef RECIPEBOOK_SERIALIZATION_JSON_JSONREADERV1_H
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
    class JsonReaderV1 final
    {
    public:
        bool read(const QJsonArray& rRootObject, RBMetaData& rMetaData, RecipeBook& rRecipeBook);

    private:
        JsonReaderV1();

        bool readMetadata(const QJsonObject& rObject, RBMetaData& rMetaData);

        bool readCategories(const QJsonObject& rObject, RecipeBook& rRecipeBook);
        bool readIngredients(const QJsonObject& rObject, RecipeBook& rRecipeBook);
        bool readRecipes(const QJsonObject& rObject, RecipeBook& rRecipeBook);
        bool readShoppongList(const QJsonObject& rObject, RecipeBook& rRecipeBook);

        bool readRecipeItemGroup(const QJsonObject& rObject, RecipeBook& rRecipeBook, Recipe& rRecipe);
        
        friend class JsonReader;
    };
}

#endif
