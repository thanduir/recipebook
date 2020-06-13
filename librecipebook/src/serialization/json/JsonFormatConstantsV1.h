#ifndef RECIPEBOOK_SERIALIZATION_JSONFORMATCONSTANTSV1_H
#define RECIPEBOOK_SERIALIZATION_JSONFORMATCONSTANTSV1_H

namespace recipebook::serialization::json
{
	// top level ids
	constexpr char* c_strCategoriesId					= "Categories";
	constexpr char* c_strIngredientsId					= "Ingredients";
	constexpr char* c_strRecipesId						= "Recipes";
	constexpr char* c_strShoppinglistId					= "Shoppinglist";

	// metadata
	constexpr char* c_strId								= "id";
	constexpr char* c_strMetaDataId						= "";
	constexpr char* c_strOriginId						= "origin";
	constexpr char* c_strVersion						= "version";
    
	// categories
	constexpr char* c_strCategoriesAll					= "all categories";
	constexpr char* c_strCategoriesSortOrders			= "sortOrders";

	// ingredients
	constexpr char* c_strIngredientsCategory			= "category";
	constexpr char* c_strIngredientsProvenance			= "provenance";
	constexpr char* c_strIngredientsDefaultUnit			= "default-unit";

	constexpr char* c_strProvenanceEverywhere			= "*EVERYWHERE*";

	// recipes
	constexpr char* c_strRecipesActiveRecipe			= "activeRecipe";    
	constexpr char* c_strRecipesNrPersons				= "NrPersons";
	constexpr char* c_strRecipesGroup					= "group";
    
	// recipe groups
	constexpr char* c_strRecipesGroupName				= "groupName";

	// shopping list
	constexpr char* c_strShoppinglistSortOrder			= "currentSortOrder";

	// shopping recipes
	constexpr char* c_strShoppingRecipesScalingFactor	= "ScalingFactor";
	constexpr char* c_strShoppingRecipesDueDate			= "DueDate";

	// (shopping) recipe item
	constexpr char* c_strRecipesAmount					= "amount";
	constexpr char* c_strRecipesAmountMinMax			= "amountMinMax";
	constexpr char* c_strRecipesSize					= "size";
	constexpr char* c_strRecipesOptional				= "optional";
	constexpr char* c_strRecipesAdditionalInfo			= "additionalInfo";
	constexpr char* c_strRecipesStatus					= "status";
}

#endif
