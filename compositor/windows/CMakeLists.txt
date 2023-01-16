cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME "expidus_runtime_compositor")
project(${PROJECT_NAME} LANGUAGES CXX)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_BINARY_DIR}/shared")

set(expidus_runtime_compositor_bundled_libraries
  $<TARGET_FILE:expidus_runtime_compositor>
  PARENT_SCOPE)
