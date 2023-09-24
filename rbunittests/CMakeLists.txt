find_package(Qt6 COMPONENTS Core Gui Network PrintSupport Qml Quick QuickControls2 Svg LinguistTools Test REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin_unittests)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin_unittests)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin_unittests)

enable_testing(true)

add_executable(rbunittests ${LIBRECIPEBOOK_FILES} src/main.cpp src/RBUnitTests.cpp rbunittests.qrc)
add_test(NAME rbunittests COMMAND rbunittests)

target_include_directories(rbunittests PRIVATE "${PROJECT_SOURCE_DIR}/librecipebook/include")

target_link_libraries(rbunittests PRIVATE Qt6::Test Qt6::Core Qt6::Gui Qt6::Network Qt6::PrintSupport Qt6::Qml Qt6::Quick Qt6::QuickControls2 Qt6::Svg)

if(WIN32)
	find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${MY_QT_PATH}/bin")
	add_custom_command(TARGET rbunittests POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E
			env PATH="${MY_QT_PATH}/bin" "${WINDEPLOYQT_EXECUTABLE}"
			"$<TARGET_FILE:rbunittests>"
		COMMENT "Running windeployqt...")
endif()

target_compile_options(rbunittests PRIVATE 
					$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
						-Wshadow -Wall -Wextra -Wsign-conversion -Wconversion -pedantic -Werror>
					$<$<CXX_COMPILER_ID:MSVC>:
						/W3 /WX>)
