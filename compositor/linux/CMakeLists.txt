cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME "expidus_runtime_compositor")
project(${PROJECT_NAME} LANGUAGES C)

set(PLUGIN_NAME "expidus_runtime_compositor")

add_library(${PLUGIN_NAME} SHARED "src/compositor.c")
apply_standard_settings(${PLUGIN_NAME})
target_include_directories(${PLUGIN_NAME} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(expidus_runtime_compositor_bundled_libraries "" PARENT_SCOPE)
