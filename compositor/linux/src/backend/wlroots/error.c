#include <expidus/runtime/compositor/backend/wlroots/error.h>
#include "error-priv.h"

typedef struct {
  gpointer padding[12];
} ExpidusRuntimeCompositorWlrootsErrorPrivate;

static void expidus_runtime_compositor_wlroots_error_private_init(ExpidusRuntimeCompositorWlrootsErrorPrivate* priv) {}

static void expidus_runtime_compositor_wlroots_error_private_copy(const ExpidusRuntimeCompositorWlrootsErrorPrivate* src_priv, ExpidusRuntimeCompositorWlrootsErrorPrivate* dest_priv) {}

static void expidus_runtime_compositor_wlroots_error_private_clear(ExpidusRuntimeCompositorWlrootsErrorPrivate* priv) {}

G_DEFINE_EXTENDED_ERROR(ExpidusRuntimeCompositorWlrootsError, expidus_runtime_compositor_wlroots_error);

void expidus_runtime_compositor_wlroots_error_set_drm(GError** error) {
  g_set_error(error, EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_ERROR, EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_ERROR_DRM, "Failed to open render node");
}

void expidus_runtime_compositor_wlroots_error_set_wayland(GError** error, const gchar* msg) {
  g_set_error(error, EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_ERROR, EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_ERROR_WAYLAND, "A wayland operation has failed: %s", msg);
}

void expidus_runtime_compositor_wlroots_error_set_renderer(GError** error, const gchar* msg) {
  g_set_error(error, EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_ERROR, EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_ERROR_RENDERER, "A rendering operation has failed: %s", msg);
}
