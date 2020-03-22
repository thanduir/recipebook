#ifndef RECIPEBOOK_SERIALIZATION_JSON_JSONWRITER_H
#define RECIPEBOOK_SERIALIZATION_JSON_JSONWRITER_H

#include "../IRecipeBookSerializer.h"

namespace recipebook
{
    class RecipeItemGroup;
}

namespace recipebook::serialization::json
{
    class JsonWriter final : public recipebook::serialization::IRBWriter
    {
    public:
        virtual bool serialize(const RecipeBook& rRecipeBook, QFile& file) override final;

    private:
        JsonWriter(QString strUID, bool bStripDescriptions);

        void writeMetadata(const RBMetaData& rMetaData, QJsonObject& rRootArray);

        void writeCategories(const RecipeBook& rRecipeBook, QJsonObject& rRootArray);
        void writeIngredients(const RecipeBook& rRecipeBook, QJsonObject& rRootArray);
        void writeRecipes(const RecipeBook& rRecipeBook, QJsonObject& rRootArray);
        void writeShoppongList(const RecipeBook& rRecipeBook, QJsonObject& rRootArray);

        void writeRecipeItemGroup(const RecipeItemGroup& rGroup, const RecipeBook& rRecipeBook, QJsonObject& rObject);

        friend class QSharedPointer<JsonWriter>;

    private:
        QString m_strUID;
        bool m_bStripDescriptions;
    };
}

#endif
