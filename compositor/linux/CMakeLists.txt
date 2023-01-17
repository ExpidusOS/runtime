cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME "expidus_runtime_compositor")
project(${PROJECT_NAME} LANGUAGES C)

set(PLUGIN_NAME "expidus_runtime_compositor")
set(CFLAGS "-DG_LOG_DOMAIN=\"ExpidusRuntimeCompositor\"")
set(EXTRA_SOURCES "")

find_package(PkgConfig REQUIRED)

pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET glib-2.0)
pkg_check_modules(GOBJECT REQUIRED IMPORTED_TARGET gobject-2.0)
pkg_check_modules(GIO REQUIRED IMPORTED_TARGET gio-2.0)

pkg_check_modules(FLUTTER_EMBEDDER REQUIRED IMPORTED_TARGET flutter_embedder)
pkg_check_modules(WLROOTS IMPORTED_TARGET wlroots)
pkg_check_modules(XCB IMPORTED_TARGET xcb)

if(NOT WLROOTS_FOUND AND NOT XCB_FOUND)
  message(FATAL_ERROR "Missing either wlroots or xcb, cannot build without a backend library")
endif()

if(WLROOTS_FOUND)
  pkg_check_modules(WAYLAND_SERVER REQUIRED IMPORTED_TARGET wayland-server)
  pkg_check_modules(UDEV REQUIRED IMPORTED_TARGET libudev)
endif()

find_library(LIBFLUTTER_ENGINE NAMES ${FLUTTER_EMBEDDER_LIBRARIES} PATHS ${FLUTTER_EMBEDDER_LIBRARY_DIRS})

set(PLUGIN_REQIURE PkgConfig::GLIB PkgConfig::GOBJECT PkgConfig::GIO PkgConfig::FLUTTER_EMBEDDER)

function(bin_version IN_VAR OUT_VAR)
  string(REPLACE "." ";" VERSION_LIST "${IN_VAR}")
  list(REMOVE_AT VERSION_LIST 0)
  list(GET VERSION_LIST 0 VERSION_MINOR)
  list(GET VERSION_LIST 1 VERSION_PATCH)
  set(${OUT_VAR} "0.${VERSION_MINOR}.${VERSION_PATCH}" PARENT_SCOPE)
endfunction()

function(glib_version IN_VAR OUT_VAR)
  string(REPLACE "." ";" VERSION_LIST "${IN_VAR}")
  list(REMOVE_AT VERSION_LIST 0)
  list(GET VERSION_LIST 0 VERSION_MINOR)
  list(GET VERSION_LIST 1 VERSION_PATCH)
  set(${OUT_VAR} "0.${VERSION_MINOR}00.${VERSION_PATCH}" PARENT_SCOPE)
endfunction()

function(require_glib NAME)
  find_library(${NAME} NAMES ${${NAME}_LIBRARIES} PATHS ${${NAME}_LIBRARY_DIRS})
  glib_version("${${NAME}_VERSION}" ${NAME}_VERSION_REAL)
  set(PLUGIN_LIBS "${PLUGIN_LIBS}" ${${NAME}} ${${NAME}}.0 ${${NAME}}.${${NAME}_VERSION_REAL} PARENT_SCOPE)
endfunction()

set(PLUGIN_LIBS "${LIBFLUTTER_ENGINE}")
require_glib(GLIB)
require_glib(GOBJECT)
require_glib(GIO)

if(WLROOTS_FOUND)
  list(APPEND PLUGIN_REQIURE PkgConfig::WLROOTS PkgConfig::WAYLAND_SERVER PkgConfig::UDEV)
  find_library(LIBWLROOTS NAMES ${WLROOTS_LIBRARIES} PATHS ${WLROOTS_LIBRARY_DIRS})
  find_library(LIBWAYLAND_SERVER NAMES ${WAYLAND_SERVER_LIBRARIES} PATHS ${WAYLAND_SERVER_LIBRARY_DIRS})
  find_library(LIBUDEV NAMES ${UDEV_LIBRARIES} PATHS ${UDEV_LIBRARY_DIRS})

  bin_version("${WAYLAND_SERVER_VERSION}" LIBWAYLAND_SERVER_VERSION_REAL)
  set(PLUGIN_LIBS "${PLUGIN_LIBS}" ${LIBWLROOTS} ${LIBWLROOTS}.10)
  set(PLUGIN_LIBS "${PLUGIN_LIBS}" ${LIBWAYLAND_SERVER} ${LIBWAYLAND_SERVER}.0 ${LIBWAYLAND_SERVER}.${LIBWAYLAND_SERVER_VERSION_REAL})
  set(PLUGIN_LIBS "${PLUGIN_LIBS}" ${LIBUDEV} ${LIBUDEV}.1 ${LIBUDEV}.1.7.4)
  set(CFLAGS "${CFLAGS}" -DHAS_WLROOTS=1 -DWLR_USE_UNSTABLE=1)
  set(EXTRA_SOURCES "${EXTRA_SOURCES}" "src/backend/wlroots/backend.c")
endif()

if(XCB_FOUND)
  list(APPEND PLUGIN_REQIURE PkgConfig::XCB)
  find_library(LIBXCB NAMES ${XCB_LIBRARIES} PATHS ${XCB_LIBRARY_DIRS})

  set(PLUGIN_LIBS "${PLUGIN_LIBS}" ${LIBXCB} ${LIBXCB}.1 ${LIBXCB}.1.1.0)
  set(CFLAGS "${CFLAGS}" -DHAS_XCB=1)
  set(EXTRA_SOURCES "${EXTRA_SOURCES}" "src/backend/xcb/backend.c")
endif()

add_library(${PLUGIN_NAME} SHARED "src/backend.c" "src/compositor.c" ${EXTRA_SOURCES})
apply_standard_settings(${PLUGIN_NAME})
target_include_directories(${PLUGIN_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_options(${PLUGIN_NAME} PUBLIC "${CFLAGS}")
target_link_libraries(${PLUGIN_NAME} PUBLIC ${PLUGIN_REQIURE})

set(expidus_runtime_compositor_bundled_libraries "${PLUGIN_LIBS}" PARENT_SCOPE)
