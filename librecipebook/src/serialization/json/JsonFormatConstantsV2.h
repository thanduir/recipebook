#ifndef RECIPEBOOK_SERIALIZATION_JSONFORMATCONSTANTSV2_H
#define RECIPEBOOK_SERIALIZATION_JSONFORMATCONSTANTSV2_H

namespace recipebook::serialization::json
{
	// top level headers
	constexpr char* c_strMetadataId						= "Metadata";
	constexpr char* c_strAlternativesTypesId			= "alternativesTypes";
	constexpr char* c_strCategoriesId					= "categories";
	constexpr char* c_strIngredientsId					= "ingredients";
	constexpr char* c_strRecipesId						= "recipes";
	constexpr char* c_strShoppinglistId					= "shoppinglist";
	constexpr char* c_strRBConfigId						= "z_recipebookconfig";

	// metadata
	constexpr char* c_strMetaDataId						= "id";
	constexpr char* c_strOriginSW						= "origin-sw";
	constexpr char* c_strOriginUID						= "origin-uid";
	constexpr char* c_strVersion						= "version";
        
	// categories
	constexpr char* c_strCategoriesAll					= "list";
	constexpr char* c_strCategoriesSortOrders			= "sortOrders";
        
	// ingredients
	constexpr char* c_strIngredientsCategory			= "category";
	constexpr char* c_strIngredientsUnavailProvenances	= "unavailableProvenances";
	constexpr char* c_strIngredientsDefaultUnit			= "default-unit";
    
	// alternatives types
	constexpr char* c_strAlternativesColor				= "color";

	// recipes
	constexpr char* c_strRecipesNrPersons				= "NrPersons";
	constexpr char* c_strRecipesShortDesc				= "ShortDesc";
	constexpr char* c_strRecipesText					= "Text";
	constexpr char* c_strRecipesCookingTime				= "CookingTime";
	constexpr char* c_strRecipesItems					= "items";

	// shopping recipe
	constexpr char* c_strShoppingRecipesScalingFactor	= "ScalingFactor";
	constexpr char* c_strShoppingRecipesDueDate			= "DueDate";

	// (shopping) recipe item
	constexpr char* c_strRecipesAmount					= "amount";
	constexpr char* c_strRecipesAmountMin				= "min";
	constexpr char* c_strRecipesAmountMax				= "max";
	constexpr char* c_strRecipesAmountUnit				= "unit";
	constexpr char* c_strRecipesSize					= "size";
	constexpr char* c_strRecipesOptional				= "optional";
	constexpr char* c_strRecipesAdditionalInfo			= "additionalInfo";
	constexpr char* c_strRecipesStatus					= "status";
	constexpr char* c_strRecipesGroup					= "alternativesGroup";
	constexpr char* c_strRecipesItemEnabled				= "itemEnabled";
	constexpr char* c_strRecipesPosition				= "_position";

	// recipe book configurations
	constexpr char* c_strConfigTitle					= "Title";
	constexpr char* c_strConfigSubtitle					= "Subtitle";
	constexpr char* c_strConfigFontSize					= "FontSize";
	constexpr char* c_strConfigLanguageCode				= "LanguageCode";
	constexpr char* c_strConfigItems					= "items";

	// recipe boook config items
	constexpr char* c_strConfigItemPosition				= "_position";
	constexpr char* c_strConfigItemType					= "type";
	constexpr char* c_strConfigItemHeaderName			= "name";
	constexpr char* c_strConfigItemHeaderLevel			= "headerLevel";
}

#endif