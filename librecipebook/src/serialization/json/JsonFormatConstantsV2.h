#ifndef RECIPEBOOK_SERIALIZATION_JSONFORMATCONSTANTSV2_H
#define RECIPEBOOK_SERIALIZATION_JSONFORMATCONSTANTSV2_H

namespace recipebook::serialization::json
{
	// top level headers
    constexpr const char* c_strMetadataId						= "Metadata";
    constexpr const char* c_strAlternativesTypesId              = "alternativesTypes";
    constexpr const char* c_strCategoriesId                     = "categories";
    constexpr const char* c_strIngredientsId					= "ingredients";
    constexpr const char* c_strRecipesId						= "recipes";
    constexpr const char* c_strShoppinglistId					= "shoppinglist";
    constexpr const char* c_strRBConfigId						= "z_recipebookconfig";

	// metadata
    constexpr const char* c_strMetaDataId						= "id";
    constexpr const char* c_strOriginSW                         = "origin-sw";
    constexpr const char* c_strOriginUID                        = "origin-uid";
    constexpr const char* c_strVersion                          = "version";
        
	// categories
    constexpr const char* c_strCategoriesAll					= "list";
    constexpr const char* c_strCategoriesSortOrders             = "sortOrders";
        
	// ingredients
    constexpr const char* c_strIngredientsCategory              = "category";
    constexpr const char* c_strIngredientsUnavailProvenances	= "unavailableProvenances";
    constexpr const char* c_strIngredientsDefaultUnit			= "default-unit";
    
	// alternatives types
    constexpr const char* c_strAlternativesColor                = "color";

	// recipes
    constexpr const char* c_strRecipesNrPersons                 = "NrPersons";
    constexpr const char* c_strRecipesShortDesc                 = "ShortDesc";
    constexpr const char* c_strRecipesText                      = "Text";
    constexpr const char* c_strRecipesCookingTime               = "CookingTime";
    constexpr const char* c_strRecipesItems                     = "items";

	// shopping recipe
    constexpr const char* c_strShoppingRecipesScalingFactor     = "ScalingFactor";
    constexpr const char* c_strShoppingRecipesDueDate           = "DueDate";

	// (shopping) recipe item
    constexpr const char* c_strRecipesAmount					= "amount";
    constexpr const char* c_strRecipesAmountMin                 = "min";
    constexpr const char* c_strRecipesAmountMax                 = "max";
    constexpr const char* c_strRecipesAmountUnit				= "unit";
    constexpr const char* c_strRecipesSize                      = "size";
    constexpr const char* c_strRecipesOptional                  = "optional";
    constexpr const char* c_strRecipesAdditionalInfo			= "additionalInfo";
    constexpr const char* c_strRecipesStatus					= "status";
    constexpr const char* c_strRecipesGroup                     = "alternativesGroup";
    constexpr const char* c_strRecipesItemEnabled				= "itemEnabled";
    constexpr const char* c_strRecipesPosition                  = "_position";

	// recipe book configurations
    constexpr const char* c_strConfigTitle                      = "Title";
    constexpr const char* c_strConfigSubtitle					= "Subtitle";
    constexpr const char* c_strConfigFontSize					= "FontSize";
    constexpr const char* c_strConfigLanguageCode				= "LanguageCode";
    constexpr const char* c_strConfigItems                      = "items";

	// recipe boook config items
    constexpr const char* c_strConfigItemPosition				= "_position";
    constexpr const char* c_strConfigItemType					= "type";
    constexpr const char* c_strConfigItemHeaderName             = "name";
    constexpr const char* c_strConfigItemHeaderLevel			= "headerLevel";
}

#endif
