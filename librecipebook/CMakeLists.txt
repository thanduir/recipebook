# TODO: Unify files list!
if(ANDROID)

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

                            PARENT_SCOPE)

else()

    find_package(Qt5 COMPONENTS Core Gui REQUIRED)

    add_library(librecipebook STATIC
                                    src/data/Amount.cpp src/data/GoShoppingListItem.cpp src/data/Ingredient.cpp
                                    src/data/RBDataHandler.cpp src/data/Recipe.cpp src/data/RecipeBook.cpp
                                    src/data/RecipeBookConfigItem.cpp src/data/RecipeBookConfiguration.cpp
                                    src/data/RecipeItem.cpp src/data/ShoppingListItem.cpp src/data/ShoppingRecipe.cpp
                                    src/data/SortedShoppingList.cpp src/data/SortOrder.cpp src/data/Unit.cpp

                                   src/serialization/RecipeBookSerializerFactory.cpp src/serialization/StringConverter.cpp

                                    src/serialization/json/RecipeBookJsonReader.cpp
                                    src/serialization/json/RecipeBookJsonReaderV1.cpp
                                    src/serialization/json/RecipeBookJsonReaderV2.cpp
                                    src/serialization/json/RecipeBookJsonWriter.cpp)

    target_include_directories(librecipebook PRIVATE "include")

    target_link_libraries(librecipebook Qt5::Core Qt5::Gui)
endif()
