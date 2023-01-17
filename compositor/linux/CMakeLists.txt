cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME "expidus_runtime_compositor")
project(${PROJECT_NAME} LANGUAGES C)

set(PLUGIN_NAME "expidus_runtime_compositor")

find_package(PkgConfig REQUIRED)
pkg_check_modules(FLUTTER_EMBEDDER IMPORTED_TARGET flutter_embedder)
pkg_check_modules(WLROOTS IMPORTED_TARGET wlroots)
pkg_check_modules(XCB IMPORTED_TARGET xcb)

if(NOT WLROOTS_FOUND AND NOT XCB_FOUND)
  message(FATAL_ERROR "Missing either wlroots or xcb, cannot build without a backend library")
endif()

set(PLUGIN_LIBS PkgConfig::FLUTTER_EMBEDDER)

if(WLROOTS_FOUND)
  list(APPEND PLUGIN_LIBS PkgConfig::WLROOTS)
endif()

if(XCB_FOUND)
  list(APPEND PLUGIN_LIBS PkgConfig::XCB)
endif()

add_library(${PLUGIN_NAME} SHARED "src/compositor.c")
apply_standard_settings(${PLUGIN_NAME})
target_include_directories(${PLUGIN_NAME} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${PLUGIN_NAME} PUBLIC ${PLUGIN_LIBS})

set(expidus_runtime_compositor_bundled_libraries "" PARENT_SCOPE)
