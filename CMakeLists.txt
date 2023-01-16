cmake_minimum_required(VERSION 3.10)

project(expidus_runtime_library VERSION 0.1.0 LANGUAGES C)

add_library(expidus_runtime SHARED "src/runtime.c")

set_target_properties(expidus_runtime PROPERTIES
  PUBLIC_HEADER include/expidus/runtime.h
  OUTPUT_NAME "expidus_runtime")

target_compile_definitions(expidus_runtime PUBLIC DART_SHARED_LIB)

if(WLROOTS_FOUND OR XCB_FOUND)
  add_subdirectory(compositor)
endif()
