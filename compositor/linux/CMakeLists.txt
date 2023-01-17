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

set(PLUGIN_REQIURE PkgConfig::FLUTTER_EMBEDDER)
find_library(LIBFLUTTER_ENGINE NAMES ${FLUTTER_EMBEDDER_LIBRARIES} PATHS ${FLUTTER_EMBEDDER_LIBRARY_DIRS})
set(PLUGIN_LIBS "${LIBFLUTTER_ENGINE}")

if(WLROOTS_FOUND)
  list(APPEND PLUGIN_REQIURE PkgConfig::WLROOTS)
  find_library(LIBWLROOTS NAMES ${WLROOTS_LIBRARIES} PATHS ${WLROOTS_LIBRARY_DIRS})
  set(PLUGIN_LIBS "${PLUGIN_LIBS}" ${LIBWLROOTS})
  set(PLUGIN_LIBS "${PLUGIN_LIBS}" ${LIBWLROOTS}.10)
endif()

if(XCB_FOUND)
  list(APPEND PLUGIN_REQIURE PkgConfig::XCB)
  find_library(LIBXCB NAMES ${XCB_LIBRARIES} PATHS ${XCB_LIBRARY_DIRS})
  set(PLUGIN_LIBS "${PLUGIN_LIBS}" ${LIXCB})
endif()

add_library(${PLUGIN_NAME} SHARED "src/compositor.c")
apply_standard_settings(${PLUGIN_NAME})
target_include_directories(${PLUGIN_NAME} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${PLUGIN_NAME} PUBLIC ${PLUGIN_REQIURE})

set(expidus_runtime_compositor_bundled_libraries "${PLUGIN_LIBS}" PARENT_SCOPE)
