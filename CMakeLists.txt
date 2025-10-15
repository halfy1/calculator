cmake_minimum_required(VERSION 3.10)
project(Calculator)

set(CMAKE_CXX_STANDARD 17)

add_executable(calc
    main.cpp
    Calculator.cpp
    ExpressionParser.cpp
    PluginManager.cpp
    SharedLibrary.cpp
)

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