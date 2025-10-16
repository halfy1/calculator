cmake_minimum_required(VERSION 3.14)
project(Calculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cpp
    src/Calculator.cpp
    src/ExpressionParser.cpp
    src/PluginManager.cpp
    src/SharedLibrary.cpp
)

add_executable(calc src/main.cpp ${SOURCES})
target_include_directories(calc PRIVATE src)
target_link_libraries(calc dl)



file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)

file(GLOB PLUGIN_SOURCES "plugins/func*.cpp")
foreach(plugin_src ${PLUGIN_SOURCES})
    get_filename_component(plugin_name ${plugin_src} NAME_WE)
    add_library(${plugin_name} SHARED ${plugin_src})
    set_target_properties(${plugin_name} PROPERTIES 
        PREFIX ""
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
    )
    target_link_libraries(${plugin_name} m)
endforeach()

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests
    tests/test_parser.cpp
    tests/test_calculator.cpp
    tests/test_plugins.cpp
    src/ExpressionParser.cpp
    src/PluginManager.cpp
    src/SharedLibrary.cpp
)

target_include_directories(tests PRIVATE src)
target_link_libraries(tests gtest gtest_main dl)

include(GoogleTest)