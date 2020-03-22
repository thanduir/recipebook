#ifndef RECIPEBOOK_SERIALIZATION_JSON_JSONREADERV2_H
#define RECIPEBOOK_SERIALIZATION_JSON_JSONREADERV2_H

#include "../IRecipeBookSerializer.h"

namespace recipebook
{
    class Ingredient;
    class Recipe;
    class RecipeItemGroup;
}

namespace recipebook::serialization::json
{
    class JsonReaderV2 final
    {
    public:
        bool read(QJsonObject& rRootObject, RBMetaData& rMetaData, RecipeBook& rRecipeBook);

    private:
        JsonReaderV2();

        bool readMetadata(const QJsonObject& rObject, RBMetaData& rMetaData);

        bool readCategories(const QJsonObject& rObject, RecipeBook& rRecipeBook);
        bool readIngredients(const QJsonObject& rObject, RecipeBook& rRecipeBook);
        bool readRecipes(const QJsonObject& rObject, RecipeBook& rRecipeBook);
        bool readShoppongList(const QJsonObject& rObject, RecipeBook& rRecipeBook);

        bool readRecipeItemGroup(const QJsonObject& rObject, QString strGroupName, RecipeBook& rRecipeBook, Recipe& rRecipe);
        
        friend class JsonReader;
    };
}

#endif
