#ifndef RECIPEBOOK_SERIALIZATION_JSON_JSONREADER_H
#define RECIPEBOOK_SERIALIZATION_JSON_JSONREADER_H

#include "serialization/IRecipeBookSerializer.h"

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

		virtual void enableAlternativesGroupsSorting() override final { m_bUseAlternativesGroupsSorting = true; }
		virtual void disableAlternativesGroupsSorting() override final { m_bUseAlternativesGroupsSorting = false; }

    private:
        JsonReader();
        
	private:
		bool m_bUseAlternativesGroupsSorting = true;

        friend class QSharedPointer<JsonReader>;
    };
}

#endif
