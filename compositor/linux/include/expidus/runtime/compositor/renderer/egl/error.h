#pragma once

#include <glib.h>

typedef enum {
  EXPIDUS_RUNTIME_COMPOSITOR_EGL_ERROR_SHADER,
} ExpidusRuntimeCompositorEglError;

#define EXPIDUS_RUNTIME_COMPOSITOR_EGL_ERROR expidus_runtime_compositor_egl_error_quark()
GQuark expidus_runtime_compositor_egl_error_quark();
