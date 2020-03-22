#ifndef RECIPEBOOK_SERIALIZATION_JSONFORMATCONSTANTSV2_H
#define RECIPEBOOK_SERIALIZATION_JSONFORMATCONSTANTSV2_H

namespace recipebook::serialization::json
{
    // top level headers
    constexpr char* c_strMetadataId                     = "Metadata";
    constexpr char* c_strCategoriesId                   = "categories";
    constexpr char* c_strIngredientsId                  = "ingredients";
    constexpr char* c_strRecipesId                      = "recipes";
    constexpr char* c_strShoppinglistId                 = "shoppinglist";

    // metadata
    constexpr char* c_strMetaDataId                     = "id";
    constexpr char* c_strOriginSW                       = "origin-sw";
    constexpr char* c_strOriginUID                      = "origin-uid";
    constexpr char* c_strVersion                        = "version";
        
    // categories
    constexpr char* c_strCategoriesAll                  = "list";
    constexpr char* c_strCategoriesSortOrders           = "sortOrders";
        
    // ingredients
    constexpr char* c_strIngredientsCategory            = "category";
    constexpr char* c_strIngredientsProvenance          = "provenance";
    constexpr char* c_strIngredientsDefaultUnit         = "default-unit";
    
    // recipes
    constexpr char* c_strRecipesNrPersons               = "NrPersons";
    constexpr char* c_strRecipesGroups                  = "groups";
    constexpr char* c_strRecipesItems                   = "items";

    // shopping recipe
    constexpr char* c_strShoppingRecipesScalingFactor   = "ScalingFactor";
    constexpr char* c_strShoppingRecipesDueDate         = "DueDate";

    // (shopping) recipe item
    constexpr char* c_strRecipesAmount                  = "amount";
    constexpr char* c_strRecipesAmountMin               = "min";
    constexpr char* c_strRecipesAmountMax               = "max";
    constexpr char* c_strRecipesAmountUnit              = "unit";
    constexpr char* c_strRecipesSize                    = "size";
    constexpr char* c_strRecipesOptional                = "optional";
    constexpr char* c_strRecipesAdditionalInfo          = "additionalInfo";
    constexpr char* c_strRecipesStatus                  = "status";
}

#endif
