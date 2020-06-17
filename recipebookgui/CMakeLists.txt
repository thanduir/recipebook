find_package(Qt5 COMPONENTS Core Gui PrintSupport Qml Quick QuickControls2 Svg LinguistTools REQUIRED)

set(TS_FILES translations/recipebookgui_de.ts translations/recipebookgui_en.ts)

set(RECIPEBOOK_FILES    src/main.cpp src/RBDialogInterface.cpp src/RBTranslationManager.cpp
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
                        src/exporter/ShoppingListExporter.cpp src/exporter/ShoppingListExporterLatex.cpp)

qt5_create_translation(QM_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${TS_FILES})

if(NOT ANDROID)

    add_executable(recipebook WIN32 ${RECIPEBOOK_FILES} ${TS_FILES} ${QM_FILES}
                                    recipebookgui.qrc
                                    icon/icon.rc)

    target_include_directories(recipebook PRIVATE "${PROJECT_SOURCE_DIR}/librecipebook/include")

    set_target_properties(recipebook PROPERTIES DEBUG_POSTFIX "D")

    target_link_libraries(recipebook librecipebook Qt5::Core Qt5::Gui Qt5::PrintSupport Qt5::Qml Qt5::Quick Qt5::QuickControls2 Qt5::Svg)

    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${MY_QT_PATH}/bin")
    add_custom_command(TARGET recipebook POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E
            env PATH="${MY_QT_PATH}/bin" "${WINDEPLOYQT_EXECUTABLE}"
                    --qmldir "${CMAKE_CURRENT_SOURCE_DIR}/resources/qml-desktop"
                    "$<TARGET_FILE:recipebook>"
        COMMENT "Running windeployqt...")
else()

    add_library(recipebook SHARED ${RECIPEBOOK_FILES} ${LIBRECIPEBOOK_FILES} ${TS_FILES} ${QM_FILES}
                            recipebookgui.qrc)

    target_include_directories(recipebook PRIVATE "${PROJECT_SOURCE_DIR}/librecipebook/include")

    target_link_libraries(recipebook Qt5::Core Qt5::Gui Qt5::PrintSupport Qt5::Qml Qt5::Quick Qt5::QuickControls2 Qt5::Svg)

endif()

foreach(file_i ${QM_FILES})
        add_custom_command(
                TARGET recipebook POST_BUILD VERBATIM
                COMMAND ${CMAKE_COMMAND} -E copy "${file_i}" ${CMAKE_CURRENT_SOURCE_DIR}/translations/)
endforeach(file_i)

target_compile_options(recipebook PRIVATE 
					$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
						-Wshadow -Wall -Wextra -Wsign-conversion -Wconversion -pedantic -Werror>
					$<$<CXX_COMPILER_ID:MSVC>:
						/W4 /WX>)
