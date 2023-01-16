cmake_minimum_required(VERSION 3.10)

project(expidus_runtime_compositor_library VERSION 0.1.0 LANGUAGES C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(WLROOTS IMPORTED_TARGET wlroots)
pkg_check_modules(XCB IMPORTED_TARGET xcb)

set(EXPIDUS_RUNTIME_COMPOSITOR_LIBS "")
set(EXPIDUS_RUNTIME_COMPOSITOR_CFLAGS "")
set(EXPIDUS_RUNTIME_COMPOSITOR_SOURCES "src/runtime.c")

if(WLROOTS_FOUND)
  list(APPEND EXPIDUS_RUNTIME_COMPOSITOR_LIBS PkgConfig::WLROOTS)
  list(APPEND EXPIDUS_RUNTIME_COMPOSITOR_CFLAGS "-DHAS_WLROOTS")
endif()

if(XCB_FOUND)
  list(APPEND EXPIDUS_RUNTIME_COMPOSITOR_LIBS PkgConfig::XCB)
  list(APPEND EXPIDUS_RUNTIME_COMPOSITOR_CFLAGS "-DHAS_XCB")
endif()

add_library(expidus_runtime_compositor SHARED ${EXPIDUS_RUNTIME_COMPOSITOR_SOURCES})

set_target_properties(expidus_runtime_compositor PROPERTIES
  PUBLIC_HEADER include/expidus/runtime/compositor.h
  OUTPUT_NAME "expidus_runtime_compositor")

string(REPLACE ";" " " EXPIDUS_RUNTIME_COMPOSITOR_CFLAGS_STRING "${EXPIDUS_RUNTIME_COMPOSITOR_CFLAGS_STRING}")
set_target_properties(expidus_runtime_compositor PROPERTIES
  COMPILE_FLAGS "${EXPIDUS_RUNTIME_COMPOSITOR_CFLAGS_STRING}")

target_compile_definitions(expidus_runtime_compositor PUBLIC DART_SHARED_LIB)
target_include_directories(expidus_runtime_compositor PUBLIC "include")

target_link_libraries(expidus_runtime_compositor PUBLIC ${EXPIDUS_RUNTIME_COMPOSITOR_LIBS})
