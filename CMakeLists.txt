cmake_minimum_required(VERSION 3.18)

project(RecipeBook VERSION 0.5)

# Qt path
if(ANDROID)
    set(MY_QT_PATH "C:/Qt/6.0.3/android")
else()
    set(MY_QT_PATH "C:/Qt/6.0.3/msvc2019_64")
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

add_subdirectory(librecipebook)
add_subdirectory(recipebookgui)

if(NOT ANDROID)
    add_subdirectory(rbunittests)
endif()
