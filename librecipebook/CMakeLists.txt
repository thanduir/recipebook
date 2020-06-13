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
