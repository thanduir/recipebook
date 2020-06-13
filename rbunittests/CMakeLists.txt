find_package(Qt5Test REQUIRED)

if (MSVC)
    add_compile_options(/W3)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin_unittests)

enable_testing(true)

add_executable(rbunittests src/main.cpp src/RBUnitTests.cpp rbunittests.qrc)
add_test(NAME rbunittests COMMAND rbunittests)

target_include_directories(rbunittests PRIVATE "${PROJECT_SOURCE_DIR}/librecipebook/include")

target_link_libraries(rbunittests PRIVATE librecipebook Qt5::Test)

find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${MY_QT_PATH}/bin")
add_custom_command(TARGET rbunittests POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E
        env PATH="${MY_QT_PATH}/bin" "${WINDEPLOYQT_EXECUTABLE}" 
		"$<TARGET_FILE:rbunittests>"
    COMMENT "Running windeployqt...")