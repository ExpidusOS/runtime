#include <expidus/runtime/compositor/backend.h>
#include <expidus/runtime/compositor.h>
#include <glib.h>
#include "backend-priv.h"

#ifdef HAS_WLROOTS
#include <expidus/runtime/compositor/backend/wlroots/backend.h>
#endif

#ifdef HAS_XCB
#include <expidus/runtime/compositor/backend/xcb/backend.h>
#endif

G_DEFINE_TYPE_WITH_PRIVATE(ExpidusRuntimeCompositorBackend, expidus_runtime_compositor_backend, G_TYPE_OBJECT);

static const gchar* expidus_runtime_compositor_backend_impl_get_name(ExpidusRuntimeCompositorBackend* self) {
  return NULL;
}

static void expidus_runtime_compositor_backend_impl_run(ExpidusRuntimeCompositorBackend* self) {}

static void expidus_runtime_compositor_backend_dispose(GObject* object) {
  ExpidusRuntimeCompositorBackend* self = EXPIDUS_RUNTIME_COMPOSITOR_BACKEND(object);
  g_assert(self != NULL);
  
  ExpidusRuntimeCompositorBackendPrivate* priv = expidus_runtime_compositor_backend_get_instance_private(self);
  g_assert(priv != NULL);

  g_clear_object(&priv->compositor);

  G_OBJECT_CLASS(expidus_runtime_compositor_backend_parent_class)->dispose(object);
}

static void expidus_runtime_compositor_backend_class_init(ExpidusRuntimeCompositorBackendClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  klass->get_name = expidus_runtime_compositor_backend_impl_get_name;
  klass->run = expidus_runtime_compositor_backend_impl_run;

  object_class->dispose = expidus_runtime_compositor_backend_dispose;
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

void expidus_runtime_compositor_backend_set_compositor(ExpidusRuntimeCompositorBackend* self, ExpidusRuntimeCompositor* compositor) {
  g_return_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_BACKEND(self));
  
  ExpidusRuntimeCompositorBackendPrivate* priv = expidus_runtime_compositor_backend_get_instance_private(self);
  g_assert(priv != NULL);

  priv->compositor = g_object_ref(compositor);
}

ExpidusRuntimeCompositor* expidus_runtime_compositor_backend_get_compositor(ExpidusRuntimeCompositorBackend* self) {
  g_return_val_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_BACKEND(self), NULL);
  
  ExpidusRuntimeCompositorBackendPrivate* priv = expidus_runtime_compositor_backend_get_instance_private(self);
  g_assert(priv != NULL);

  if (priv->compositor == NULL) return NULL;
  return g_object_ref(priv->compositor);
}

const gchar* expidus_runtime_compositor_backend_get_name(ExpidusRuntimeCompositorBackend* self) {
  g_return_val_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_BACKEND(self), NULL);

  ExpidusRuntimeCompositorBackendClass* klass = EXPIDUS_RUNTIME_COMPOSITOR_BACKEND_GET_CLASS(self);
  g_assert(klass != NULL);

  g_return_val_if_fail(klass->get_name != NULL, NULL);
  return klass->get_name(self);
}

ExpidusRuntimeCompositorRenderer* expidus_runtime_compositor_backend_get_renderer(ExpidusRuntimeCompositorBackend* self) {
  g_return_val_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_BACKEND(self), NULL);

  ExpidusRuntimeCompositorBackendClass* klass = EXPIDUS_RUNTIME_COMPOSITOR_BACKEND_GET_CLASS(self);
  g_assert(klass != NULL);

  g_return_val_if_fail(klass->get_renderer != NULL, NULL);
  return klass->get_renderer(self);
}

void expidus_runtime_compositor_backend_run(ExpidusRuntimeCompositorBackend* self) {
  g_return_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_BACKEND(self));

  ExpidusRuntimeCompositorBackendClass* klass = EXPIDUS_RUNTIME_COMPOSITOR_BACKEND_GET_CLASS(self);
  g_assert(klass != NULL);

  g_return_if_fail(klass->run != NULL);
  klass->run(self);
}

GList* expidus_runtime_compositor_backend_get_outputs(ExpidusRuntimeCompositorBackend* self) {
  g_return_val_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_BACKEND(self), NULL);

  ExpidusRuntimeCompositorBackendClass* klass = EXPIDUS_RUNTIME_COMPOSITOR_BACKEND_GET_CLASS(self);
  g_assert(klass != NULL);

  g_return_val_if_fail(klass->get_outputs != NULL, NULL);
  return klass->get_outputs(self);
}
