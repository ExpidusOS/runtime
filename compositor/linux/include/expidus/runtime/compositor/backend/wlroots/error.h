#pragma once

#include <glib.h>

typedef enum {
  EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_ERROR_DRM,
  EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_ERROR_WAYLAND,
  EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_ERROR_RENDERER,
} ExpidusRuntimeCompositorWlrootsError;

#define EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_ERROR expidus_runtime_compositor_wlroots_error_quark()
GQuark expidus_runtime_compositor_wlroots_error_quark();
