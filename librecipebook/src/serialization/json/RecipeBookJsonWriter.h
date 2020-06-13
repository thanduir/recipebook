#ifndef RECIPEBOOK_SERIALIZATION_JSON_JSONWRITER_H
#define RECIPEBOOK_SERIALIZATION_JSON_JSONWRITER_H

#include "serialization/IRecipeBookSerializer.h"

namespace recipebook
{
    class Recipe;
}

namespace recipebook::serialization::json
{
    class JsonWriter final : public recipebook::serialization::IRBWriter
    {
    public:
        virtual bool serialize(const RecipeBook& rRecipeBook, QFile& file) override final;

    private:
        JsonWriter(QString strUID, bool bJsonForApp);

		bool writeFile(QFile& file, const QJsonDocument& rJsonDoc);

        void writeMetadata(const RBMetaData& rMetaData, QJsonObject& rRootArray);

        void writeAlternativesTypes(const RecipeBook& rRecipeBook, QJsonObject& rRootObject);
        void writeCategories(const RecipeBook& rRecipeBook, QJsonObject& rRootObject);
        void writeIngredients(const RecipeBook& rRecipeBook, QJsonObject& rRootObject);
        void writeRecipes(const RecipeBook& rRecipeBook, QJsonObject& rRootObject);
        void writeShoppongList(const RecipeBook& rRecipeBook, QJsonObject& rRootObject);
		void writeRecipeBookConfigs(const RecipeBook& rRecipeBook, QJsonObject& rRootObject);

        void writeRecipeItems(const Recipe& rRecipe, QJsonObject& rObject);

        friend class QSharedPointer<JsonWriter>;

    private:
        QString m_strUID;
        bool m_bJsonForApp;
		bool m_bUseTempFile;
    };
}

#endif
