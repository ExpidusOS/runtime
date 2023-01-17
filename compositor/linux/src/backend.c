#include <expidus/runtime/compositor/backend.h>
#include <glib.h>

#ifdef HAS_WLROOTS
#include <expidus/runtime/compositor/backend/wlroots/backend.h>
#endif

#ifdef HAS_XCB
#include <expidus/runtime/compositor/backend/xcb/backend.h>
#endif

G_DEFINE_TYPE(ExpidusRuntimeCompositorBackend, expidus_runtime_compositor_backend, G_TYPE_OBJECT);

static const gchar* expidus_runtime_compositor_backend_impl_get_name(ExpidusRuntimeCompositorBackend* self) {
  return NULL;
}

static void expidus_runtime_compositor_backend_impl_run(ExpidusRuntimeCompositorBackend* self) {}

static void expidus_runtime_compositor_backend_class_init(ExpidusRuntimeCompositorBackendClass* klass) {
  klass->get_name = expidus_runtime_compositor_backend_impl_get_name;
  klass->run = expidus_runtime_compositor_backend_impl_run;
}

static void expidus_runtime_compositor_backend_init(ExpidusRuntimeCompositorBackend* self) {}

ExpidusRuntimeCompositorBackend* expidus_runtime_compositor_backend_new_auto(GCancellable* cancellable, GError** error) {
  ExpidusRuntimeCompositorBackend* self = NULL;

#ifdef HAS_WLROOTS
  if ((self = expidus_runtime_compositor_backend_new("wlroots", cancellable, error)) != NULL) return self;
#endif

#ifdef HAS_XCB
  if ((self = expidus_runtime_compositor_backend_new("xcb", cancellable, error)) != NULL) return self;
#endif
  return self;
}

ExpidusRuntimeCompositorBackend* expidus_runtime_compositor_backend_new(const gchar* name, GCancellable* cancellable, GError** error) {
#ifdef HAS_WLROOTS
  if (!g_strcmp0(name, "wlroots")) return expidus_runtime_compositor_wlroots_backend_new(cancellable, error);
#endif
#ifdef HAS_XCB
  if (!g_strcmp0(name, "xcb")) return expidus_runtime_compositor_xcb_backend_new(cancellable, error);
#endif
  return NULL;
}

const gchar* expidus_runtime_compositor_backend_get_name(ExpidusRuntimeCompositorBackend* self) {
  g_return_val_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_BACKEND(self), NULL);

  ExpidusRuntimeCompositorBackendClass* klass = EXPIDUS_RUNTIME_COMPOSITOR_BACKEND_GET_CLASS(self);
  g_assert(klass != NULL);

  g_return_val_if_fail(klass->get_name != NULL, NULL);
  return klass->get_name(self);
}

void expidus_runtime_compositor_backend_run(ExpidusRuntimeCompositorBackend* self) {
  g_return_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_BACKEND(self));

  ExpidusRuntimeCompositorBackendClass* klass = EXPIDUS_RUNTIME_COMPOSITOR_BACKEND_GET_CLASS(self);
  g_assert(klass != NULL);

  g_return_if_fail(klass->run != NULL);
  klass->run(self);
}
