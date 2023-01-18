#include <expidus/runtime/compositor/renderer/egl/error.h>
#include "error-priv.h"

typedef struct {
  gpointer padding[12];
} ExpidusRuntimeCompositorEglErrorPrivate;

static void expidus_runtime_compositor_egl_error_private_init(ExpidusRuntimeCompositorEglErrorPrivate* priv) {}

static void expidus_runtime_compositor_egl_error_private_copy(const ExpidusRuntimeCompositorEglErrorPrivate* src_priv, ExpidusRuntimeCompositorEglErrorPrivate* dest_priv) {}

static void expidus_runtime_compositor_egl_error_private_clear(ExpidusRuntimeCompositorEglErrorPrivate* priv) {}

G_DEFINE_EXTENDED_ERROR(ExpidusRuntimeCompositorEglError, expidus_runtime_compositor_egl_error);

void expidus_runtime_compositor_egl_error_set_shader(GError** error, const gchar* msg) {
  g_set_error(error, EXPIDUS_RUNTIME_COMPOSITOR_EGL_ERROR, EXPIDUS_RUNTIME_COMPOSITOR_EGL_ERROR_SHADER, "Failed to compile shaders: %s", msg);
}
