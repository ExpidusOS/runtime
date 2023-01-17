# Project-level configuration.
cmake_minimum_required(VERSION 3.14)
project(expidus_runtime_example LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/flutter/ephemeral/.plugin_symlinks/expidus_runtime/cmake")

include(ExpidusRuntime)
include(ExpidusRuntimeCompositor)

set(FLUTTER_MANAGED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/flutter")
add_expidus_compositor(BINARY_NAME "expidus_runtime_example" APPLICATION_ID "com.expidus.runtime")
