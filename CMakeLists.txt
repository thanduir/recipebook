cmake_minimum_required(VERSION 3.17)

project(RecipeBook VERSION 0.5)

# Qt path
set(MY_QT_PATH "C:/Qt/5.15.0/msvc2019_64")
#set(MY_QT_PATH "C:/Qt/5.15.0/mingw81_64")

list(APPEND CMAKE_PREFIX_PATH ${MY_QT_PATH})

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Warnings as errors and all warnings
if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wshadow -Wall -Wextra -pedantic -Werror)
endif()

# Enable Qt tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(librecipebook)
add_subdirectory(rbunittests)
add_subdirectory(recipebookgui)
