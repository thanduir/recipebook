cmake_minimum_required(VERSION 3.18)

project(RecipeBook VERSION 0.5 LANGUAGES CXX)

#######################################################################################
# General settings
#######################################################################################

# Qt path
if(ANDROID)
    set(MY_QT_PATH "C:/Qt/6.1.1/android_arm64_v8a")
else()
    set(MY_QT_PATH "C:/Qt/6.1.1/msvc2019_64")
endif()

list(APPEND CMAKE_PREFIX_PATH ${MY_QT_PATH})

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable Qt tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT ANDROID)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/Debug)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/Release)
endif()

#######################################################################################
# Files
#######################################################################################

set(LIBRECIPEBOOK_FILES ${CMAKE_CURRENT_SOURCE_DIR}/librecipebook/src/data/Amount.cpp
	                    ${CMAKE_CURRENT_SOURCE_DIR}/librecipebook/src/data/GoShoppingListItem.cpp
						${CMAKE_CURRENT_SOURCE_DIR}/librecipebook/src/data/Ingredient.cpp
						${CMAKE_CURRENT_SOURCE_DIR}/librecipebook/src/data/RBDataHandler.cpp
						${CMAKE_CURRENT_SOURCE_DIR}/librecipebook/src/data/Recipe.cpp
						${CMAKE_CURRENT_SOURCE_DIR}/librecipebook/src/data/RecipeBook.cpp
						${CMAKE_CURRENT_SOURCE_DIR}/librecipebook/src/data/RecipeBookConfigItem.cpp
						${CMAKE_CURRENT_SOURCE_DIR}/librecipebook/src/data/RecipeBookConfiguration.cpp
						${CMAKE_CURRENT_SOURCE_DIR}/librecipebook/src/data/RecipeItem.cpp
						${CMAKE_CURRENT_SOURCE_DIR}/librecipebook/src/data/ShoppingListItem.cpp
						${CMAKE_CURRENT_SOURCE_DIR}/librecipebook/src/data/ShoppingRecipe.cpp
						${CMAKE_CURRENT_SOURCE_DIR}/librecipebook/src/data/SortedShoppingList.cpp
						${CMAKE_CURRENT_SOURCE_DIR}/librecipebook/src/data/SortOrder.cpp
						${CMAKE_CURRENT_SOURCE_DIR}/librecipebook/src/data/Unit.cpp

						${CMAKE_CURRENT_SOURCE_DIR}/librecipebook/src/serialization/RecipeBookSerializerFactory.cpp
						${CMAKE_CURRENT_SOURCE_DIR}/librecipebook/src/serialization/StringConverter.cpp

						${CMAKE_CURRENT_SOURCE_DIR}/librecipebook/src/serialization/json/RecipeBookJsonReader.cpp
						${CMAKE_CURRENT_SOURCE_DIR}/librecipebook/src/serialization/json/RecipeBookJsonReaderV1.cpp
						${CMAKE_CURRENT_SOURCE_DIR}/librecipebook/src/serialization/json/RecipeBookJsonReaderV2.cpp
						${CMAKE_CURRENT_SOURCE_DIR}/librecipebook/src/serialization/json/RecipeBookJsonWriter.cpp

						${CMAKE_CURRENT_SOURCE_DIR}/librecipebook/src/synchronization/RecipebookDropbox.cpp)

set(RECIPEBOOK_FILES    recipebookgui/src/main.cpp
	                    recipebookgui/src/RBDialogInterface.cpp
						recipebookgui/src/RBTranslationManager.cpp
						recipebookgui/src/RecipeBookSettings.cpp
						recipebookgui/src/RecipeBookUIContext.cpp
						recipebookgui/src/uistringconverter.cpp

						recipebookgui/src/model/FilterModelAlternativesTypes.cpp
						recipebookgui/src/model/ListModelAlternativesGroups.cpp
						recipebookgui/src/model/ListModelRecipeBookConfigurations.cpp
						recipebookgui/src/model/ListModelSortOrders.cpp
						recipebookgui/src/model/FilterModelIngredients.cpp
						recipebookgui/src/model/ListModelCategories.cpp
						recipebookgui/src/model/ListModelRecipeItems.cpp
						recipebookgui/src/model/SortModelSortOrder.cpp
						recipebookgui/src/model/FilterModelRecipeItems.cpp
						recipebookgui/src/model/ListModelGoShopping.cpp
						recipebookgui/src/model/ListModelRecipes.cpp
						recipebookgui/src/model/FilterModelRecipes.cpp
						recipebookgui/src/model/ListModelIngredients.cpp
						recipebookgui/src/model/ListModelShoppingListItems.cpp
						recipebookgui/src/model/FilterModelUnusedRecipes.cpp
						recipebookgui/src/model/ListModelRBConfigItems.cpp
						recipebookgui/src/model/ListModelShoppingRecipes.cpp

						recipebookgui/src/synchronization/RecipebookSynchronization.cpp
						recipebookgui/src/synchronization/RecipeBookChanges.cpp
						recipebookgui/src/synchronization/RecipeBookConflicts.cpp

						recipebookgui/src/exporter/RecipeBookExporter.cpp
						recipebookgui/src/exporter/ShoppingListExporter.cpp)

set(TS_FILES			recipebookgui/translations/recipebookgui_de.ts
	                    recipebookgui/translations/recipebookgui_en.ts)

if(NOT ANDROID)
	set(QRC_FILES		recipebookgui/recipebookdesktop.qrc
						recipebookgui/icon/icon.rc)
else()
	set(QRC_FILES		recipebookgui/recipebookandroid.qrc)
endif()

#######################################################################################
# RecipeBook
#######################################################################################

# Add PoDoFo only on windows
if(NOT ANDROID)
	set(MY_PODOFO_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/3rdparty/podofo-0.9.7/src" "${PROJECT_SOURCE_DIR}/3rdparty/podofo-0.9.7/out/build/x64-Release")
	INCLUDE_DIRECTORIES(${MY_PODOFO_INCLUDE_PATH})

	set(ADDITONAL_FILES_PODOFO recipebookgui/src/exporter/RecipeBookExporterPodofo.cpp recipebookgui/src/exporter/ShoppingListExporterPodofo.cpp)

	set(MY_PODOFO_LIB_PATH "${PROJECT_SOURCE_DIR}/3rdparty/podofo-0.9.7/out/build/x64-Release/src/podofo")
	link_directories(${MY_PODOFO_LIB_PATH})
	set(ADDITIONAL_LIBS podofo)
endif()

# TODO: Remove as soon as LinguistTools are available again on android
if(NOT ANDROID)
	find_package(Qt6 COMPONENTS Core Gui Network PrintSupport Qml Quick QuickControls2 Svg LinguistTools REQUIRED)

	qt6_create_translation(QM_FILES recipebookgui/qml/desktop/recipebook.qml ${CMAKE_CURRENT_SOURCE_DIR}/recipebookgui ${TS_FILES})
	qt_add_translation(${TS_FILES})
else()
	find_package(Qt6 COMPONENTS Core Gui Network PrintSupport Qml Quick QuickControls2 Svg REQUIRED)
endif()

qt_add_executable(RecipeBook WIN32 MANUAL_FINALIZATION	${RECIPEBOOK_FILES}
														${LIBRECIPEBOOK_FILES}
														${ADDITONAL_FILES_PODOFO}
														${TS_FILES}
														${QM_FILES}
														${QRC_FILES})

target_include_directories(RecipeBook PRIVATE "${PROJECT_SOURCE_DIR}/librecipebook/include")
target_link_libraries(RecipeBook PRIVATE Qt6::Core Qt6::Gui Qt6::Network Qt6::PrintSupport Qt6::Qml Qt6::Quick Qt6::QuickControls2 Qt6::Svg ${ADDITIONAL_LIBS})

foreach(file_i ${QM_FILES})
	    add_custom_command(
			    TARGET RecipeBook POST_BUILD VERBATIM
				COMMAND ${CMAKE_COMMAND} -E copy "${file_i}" ${CMAKE_CURRENT_SOURCE_DIR}/recipebookgui/translations/)
endforeach(file_i)

target_compile_options(RecipeBook PRIVATE
	                $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
					    -Wshadow -Wall -Wextra -Wsign-conversion -Wconversion -pedantic -Werror>
					$<$<CXX_COMPILER_ID:MSVC>:
					    /W4 /WX>)

if(NOT ANDROID)
	set_target_properties(RecipeBook PROPERTIES DEBUG_POSTFIX "D")

	find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${MY_QT_PATH}/bin")
	add_custom_command(TARGET RecipeBook POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E
		    env PATH="${MY_QT_PATH}/bin" "${WINDEPLOYQT_EXECUTABLE}"
			        --qmldir "${CMAKE_CURRENT_SOURCE_DIR}/recipebookgui/qml"
					"$<TARGET_FILE:RecipeBook>"
					COMMENT "Running windeployqt...")
else()
	set_target_properties(RecipeBook PROPERTIES QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android"
		    )
endif()

qt_import_qml_plugins(RecipeBook)
qt_finalize_executable(RecipeBook)

#######################################################################################
# RBUnitTests
#######################################################################################

if(NOT ANDROID)
    add_subdirectory(rbunittests)
endif()
