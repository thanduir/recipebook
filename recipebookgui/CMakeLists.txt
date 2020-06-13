find_package(Qt5 COMPONENTS Core Gui PrintSupport Qml Quick QuickControls2 REQUIRED)

add_executable(recipebook WIN32
			src/main.cpp src/RBDialogInterface.cpp src/RBTranslationManager.cpp 
			src/RecipeBookSettings.cpp src/RecipeBookUIContext.cpp src/uistringconverter.cpp

			src/model/FilterModelAlternativesTypes.cpp src/model/ListModelAlternativesGroups.cpp
			src/model/ListModelRecipeBookConfigurations.cpp src/model/ListModelSortOrders.cpp
			src/model/FilterModelIngredients.cpp src/model/ListModelCategories.cpp
			src/model/ListModelRecipeItems.cpp src/model/SortModelSortOrder.cpp
			src/model/FilterModelRecipeItems.cpp src/model/ListModelGoShopping.cpp
			src/model/ListModelRecipes.cpp src/model/FilterModelRecipes.cpp
			src/model/ListModelIngredients.cpp src/model/ListModelShoppingListItems.cpp
			src/model/FilterModelUnusedRecipes.cpp src/model/ListModelRBConfigItems.cpp
			src/model/ListModelShoppingRecipes.cpp

			src/exporter/LatexLanguageManager.cpp src/exporter/RBLatexExporter.cpp
			src/exporter/RecipeBookExporter.cpp src/exporter/RecipeBookExporterLatex.cpp
			src/exporter/ShoppingListExporter.cpp src/exporter/ShoppingListExporterLatex.cpp
			
			recipebookgui.qrc)

target_include_directories(recipebook PRIVATE "${PROJECT_SOURCE_DIR}/librecipebook/include")

# TODO: add Translations. Also to add_executable? (at least a seem to have to add LinguistTools to find_package, right?)

set_target_properties(recipebook PROPERTIES DEBUG_POSTFIX "D")

target_link_libraries(recipebook librecipebook Qt5::Core Qt5::Gui Qt5::PrintSupport Qt5::Qml Qt5::Quick Qt5::QuickControls2)

find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${MY_QT_PATH}/bin")
add_custom_command(TARGET recipebook POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E
        env PATH="${MY_QT_PATH}/bin" "${WINDEPLOYQT_EXECUTABLE}" 
		--qmldir "${PROJECT_SOURCE_DIR}/recipebookgui/resources/qml-win" 
		"$<TARGET_FILE:recipebook>"
    COMMENT "Running windeployqt...")
