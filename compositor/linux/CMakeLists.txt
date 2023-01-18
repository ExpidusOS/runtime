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

pkg_check_modules(GLES2 IMPORTED_TARGET glesv2)
pkg_check_modules(VULKAN IMPORTED_TARGET vulkan)

pkg_check_modules(FLUTTER_EMBEDDER REQUIRED IMPORTED_TARGET flutter_embedder)
pkg_check_modules(WLROOTS IMPORTED_TARGET wlroots)
pkg_check_modules(XCB IMPORTED_TARGET xcb)

if(NOT WLROOTS_FOUND AND NOT XCB_FOUND)
  message(FATAL_ERROR "Missing either wlroots or xcb, cannot build without a backend library")
endif()

if(WLROOTS_FOUND)
  pkg_check_modules(WAYLAND_SERVER REQUIRED IMPORTED_TARGET wayland-server)
  pkg_check_modules(UDEV REQUIRED IMPORTED_TARGET libudev)
  pkg_check_modules(PIXMAN REQUIRED IMPORTED_TARGET pixman-1)
  pkg_check_modules(XKBCOMMON REQUIRED IMPORTED_TARGET xkbcommon)
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

if(GLES2_FOUND)
  list(APPEND PLUGIN_REQIURE PkgConfig::GLES2)
  find_library(LIBGLES2 NAMES ${GLES2_LIBRARIES} PATHS ${GLES2_LIBRARY_DIRS})

  set(PLUGIN_LIBS "${PLUGIN_LIBS}" ${LIBGLES2} ${LIBGLES2}.2.1.0)
  set(CFLAGS "${CFLAGS}" -DHAS_GLES2=1)
  set(EXTRA_SOURCES "${EXTRA_SOURCES}" "src/renderer/egl/renderer.c")
endif()

if(VULKAN_FOUND)
  list(APPEND PLUGIN_REQIURE PkgConfig::VULKAN)
  find_library(LIBVULKAN NAMES ${VULKAN_LIBRARIES} PATHS ${VULKAN_LIBRARY_DIRS})

  set(PLUGIN_LIBS "${PLUGIN_LIBS}" ${LIBVULKAN} ${LIBVULKAN}.1 ${LIBVULKAN}.${VULKAN_VERSION})
  set(CFLAGS "${CFLAGS}" -DHAS_VULKAN=1)
endif()

if(WLROOTS_FOUND)
  list(APPEND PLUGIN_REQIURE PkgConfig::WLROOTS PkgConfig::WAYLAND_SERVER PkgConfig::UDEV PkgConfig::PIXMAN PkgConfig::XKBCOMMON)
  find_library(LIBWLROOTS NAMES ${WLROOTS_LIBRARIES} PATHS ${WLROOTS_LIBRARY_DIRS})
  find_library(LIBWAYLAND_SERVER NAMES ${WAYLAND_SERVER_LIBRARIES} PATHS ${WAYLAND_SERVER_LIBRARY_DIRS})
  find_library(LIBUDEV NAMES ${UDEV_LIBRARIES} PATHS ${UDEV_LIBRARY_DIRS})
  find_library(LIBPIXMAN NAMES ${PIXMAN_LIBRARIES} PATHS ${PIXMAN_LIBRARY_DIRS})
  find_library(LIBXKBCOMMON NAMES ${XKBCOMMON_LIBRARIES} PATHS ${XKBCOMMON_LIBRARY_DIRS})

  bin_version("${WAYLAND_SERVER_VERSION}" LIBWAYLAND_SERVER_VERSION_REAL)
  set(PLUGIN_LIBS "${PLUGIN_LIBS}" ${LIBWLROOTS} ${LIBWLROOTS}.10)
  set(PLUGIN_LIBS "${PLUGIN_LIBS}" ${LIBWAYLAND_SERVER} ${LIBWAYLAND_SERVER}.0 ${LIBWAYLAND_SERVER}.${LIBWAYLAND_SERVER_VERSION_REAL})
  set(PLUGIN_LIBS "${PLUGIN_LIBS}" ${LIBUDEV} ${LIBUDEV}.1 ${LIBUDEV}.1.7.4)
  set(PLUGIN_LIBS "${PLUGIN_LIBS}" ${LIBPIXMAN} ${LIBPIXMAN}.0 ${LIBPIXMAN}.${PIXMAN_VERSION})
  set(PLUGIN_LIBS "${PLUGIN_LIBS}" ${LIBXKBCOMMON} ${LIBXKBCOMMON}.0 ${LIBXKBCOMMON}.0.0.0)
  set(CFLAGS "${CFLAGS}" -DHAS_WLROOTS=1 -DWLR_USE_UNSTABLE=1)
  set(EXTRA_SOURCES "${EXTRA_SOURCES}" "src/backend/wlroots/backend.c" "src/backend/wlroots/error.c" "src/backend/wlroots/output.c")

  function(add_protocol PROTOCOL BASENAME)
    pkg_get_variable(WAYLAND_PROTOCOLS_DIR wayland-protocols pkgdatadir)
    pkg_get_variable(WAYLAND_SCANNER wayland-scanner wayland_scanner)

    get_filename_component(IN_FILE ${WAYLAND_PROTOCOLS_DIR}/${PROTOCOL} ABSOLUTE)
    set(HEADER "${CMAKE_CURRENT_BINARY_DIR}/${BASENAME}-protocol.h")
    set(CODE "${CMAKE_CURRENT_BINARY_DIR}/wayland-${BASENAME}-protocol.c")

    add_custom_command(OUTPUT "${HEADER}"
      COMMAND ${WAYLAND_SCANNER} server-header < ${IN_FILE} > ${HEADER}
      DEPENDS ${IN_FILE}
      VERBATIM)

    add_custom_command(OUTPUT "${CODE}"
      COMMAND ${WAYLAND_SCANNER} code < ${IN_FILE} > ${CODE}
      DEPENDS ${IN_FILE}
      VERBATIM)

    set(EXTRA_SOURCES "${EXTRA_SOURCES}" "${HEADER}" "${CODE}" PARENT_SCOPE)
  endfunction()

  add_protocol("stable/xdg-shell/xdg-shell.xml" xdg-shell)
endif()

if(XCB_FOUND)
  list(APPEND PLUGIN_REQIURE PkgConfig::XCB)
  find_library(LIBXCB NAMES ${XCB_LIBRARIES} PATHS ${XCB_LIBRARY_DIRS})

  set(PLUGIN_LIBS "${PLUGIN_LIBS}" ${LIBXCB} ${LIBXCB}.1 ${LIBXCB}.1.1.0)
  set(CFLAGS "${CFLAGS}" -DHAS_XCB=1)
  set(EXTRA_SOURCES "${EXTRA_SOURCES}" "src/backend/xcb/backend.c")
endif()

add_library(${PLUGIN_NAME} SHARED "src/backend.c" "src/compositor.c" "src/output.c" "src/renderer.c" "src/rendering/scene-layer.c" "src/rendering/scene.c" ${EXTRA_SOURCES})
apply_standard_settings(${PLUGIN_NAME})
target_include_directories(${PLUGIN_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}")
target_compile_options(${PLUGIN_NAME} PUBLIC "${CFLAGS}")
target_link_libraries(${PLUGIN_NAME} PUBLIC ${PLUGIN_REQIURE})

set(expidus_runtime_compositor_bundled_libraries "${PLUGIN_LIBS}" PARENT_SCOPE)
