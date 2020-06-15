#ifndef RECIPEBOOK_SERIALIZATION_JSONFORMATCONSTANTSV1_H
#define RECIPEBOOK_SERIALIZATION_JSONFORMATCONSTANTSV1_H

namespace recipebook::serialization::json
{
	// top level ids
    constexpr const char* c_strCategoriesId					= "Categories";
    constexpr const char* c_strIngredientsId				= "Ingredients";
    constexpr const char* c_strRecipesId					= "Recipes";
    constexpr const char* c_strShoppinglistId				= "Shoppinglist";

	// metadata
    constexpr const char* c_strId							= "id";
    constexpr const char* c_strMetaDataId					= "";
    constexpr const char* c_strOriginId						= "origin";
    constexpr const char* c_strVersion						= "version";
    
	// categories
    constexpr const char* c_strCategoriesAll				= "all categories";
    constexpr const char* c_strCategoriesSortOrders			= "sortOrders";

	// ingredients
    constexpr const char* c_strIngredientsCategory			= "category";
    constexpr const char* c_strIngredientsProvenance		= "provenance";
    constexpr const char* c_strIngredientsDefaultUnit		= "default-unit";

    constexpr const char* c_strProvenanceEverywhere			= "*EVERYWHERE*";

	// recipes
    constexpr const char* c_strRecipesActiveRecipe			= "activeRecipe";
    constexpr const char* c_strRecipesNrPersons				= "NrPersons";
    constexpr const char* c_strRecipesGroup					= "group";
    
	// recipe groups
    constexpr const char* c_strRecipesGroupName				= "groupName";

	// shopping list
    constexpr const char* c_strShoppinglistSortOrder		= "currentSortOrder";

	// shopping recipes
    constexpr const char* c_strShoppingRecipesScalingFactor	= "ScalingFactor";
    constexpr const char* c_strShoppingRecipesDueDate		= "DueDate";

	// (shopping) recipe item
    constexpr const char* c_strRecipesAmount				= "amount";
    constexpr const char* c_strRecipesAmountMinMax			= "amountMinMax";
    constexpr const char* c_strRecipesSize					= "size";
    constexpr const char* c_strRecipesOptional				= "optional";
    constexpr const char* c_strRecipesAdditionalInfo		= "additionalInfo";
    constexpr const char* c_strRecipesStatus				= "status";
}

#endif
