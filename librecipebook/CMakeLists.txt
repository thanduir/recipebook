set(LIBRECIPEBOOK_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/data/Amount.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/data/GoShoppingListItem.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/data/Ingredient.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/data/RBDataHandler.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/data/Recipe.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/data/RecipeBook.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/data/RecipeBookConfigItem.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/data/RecipeBookConfiguration.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/data/RecipeItem.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/data/ShoppingListItem.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/data/ShoppingRecipe.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/data/SortedShoppingList.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/data/SortOrder.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/data/Unit.cpp

                        ${CMAKE_CURRENT_SOURCE_DIR}/src/serialization/RecipeBookSerializerFactory.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/serialization/StringConverter.cpp

                        ${CMAKE_CURRENT_SOURCE_DIR}/src/serialization/json/RecipeBookJsonReader.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/serialization/json/RecipeBookJsonReaderV1.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/serialization/json/RecipeBookJsonReaderV2.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/serialization/json/RecipeBookJsonWriter.cpp

						${CMAKE_CURRENT_SOURCE_DIR}/src/synchronization/RecipebookDropbox.cpp

                        PARENT_SCOPE)
