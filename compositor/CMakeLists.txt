cmake_minimum_required(VERSION 3.10)

project(expidus_runtime_compositor_library VERSION 0.1.0 LANGUAGES C)

add_library(expidus_runtime_compositor SHARED "src/runtime.c")

find_package(PkgConfig REQUIRED)
pkg_check_modules(WLROOTS IMPORTED_TARGET wlroots)
pkg_check_modules(XCB IMPORTED_TARGET xcb)

set_target_properties(expidus_runtime_compositor PROPERTIES
  PUBLIC_HEADER include/expidus/runtime/compositor.h
  OUTPUT_NAME "expidus_runtime_compositor")

target_compile_definitions(expidus_runtime_compositor PUBLIC DART_SHARED_LIB)
target_include_directories(expidus_runtime_compositor PUBLIC "../include")

if(WLROOTS_FOUND)
  target_link_libraries(expidus_runtime_compositor PUBLIC PkgConfig::WLROOTS)
endif()

if(XCB_FOUND)
  target_link_libraries(expidus_runtime_compositor PUBLIC PkgConfig::XCB)
endif()
