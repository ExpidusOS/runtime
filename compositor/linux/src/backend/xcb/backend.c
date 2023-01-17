#include <expidus/runtime/compositor/backend/xcb/backend.h>
#include <gio/gio.h>
#include "backend-priv.h"

static void g_initable_interface_init(GInitableIface* interface);

G_DEFINE_TYPE_WITH_CODE(ExpidusRuntimeCompositorXcbBackend, expidus_runtime_compositor_xcb_backend, EXPIDUS_RUNTIME_COMPOSITOR_TYPE_BACKEND,
    G_ADD_PRIVATE(ExpidusRuntimeCompositorXcbBackend)
    G_IMPLEMENT_INTERFACE(G_TYPE_INITABLE, g_initable_interface_init));

static gboolean expidus_runtime_compositor_xcb_backend_initable_init(GInitable* initable, GCancellable* cancellable, GError** error) {
  return TRUE;
}

static const gchar* expidus_runtime_compositor_xcb_backend_get_name(ExpidusRuntimeCompositorBackend* backend) {
  return "xcb";
}

static void expidus_runtime_compositor_xcb_backend_constructed(GObject* object) {
  G_OBJECT_CLASS(expidus_runtime_compositor_xcb_backend_parent_class)->constructed(object);

  ExpidusRuntimeCompositorXcbBackend* self = EXPIDUS_RUNTIME_COMPOSITOR_XCB_BACKEND(object);
  g_assert(self != NULL);

  self->priv->connection = xcb_connect(NULL, NULL);
  g_assert(self->priv->connection != NULL);
}

static void expidus_runtime_compositor_xcb_backend_dispose(GObject* object) {
  ExpidusRuntimeCompositorXcbBackend* self = EXPIDUS_RUNTIME_COMPOSITOR_XCB_BACKEND(object);
  g_assert(self != NULL);

  g_clear_pointer(&self->priv->connection, xcb_disconnect);

  G_OBJECT_CLASS(expidus_runtime_compositor_xcb_backend_parent_class)->dispose(object);
}

static void expidus_runtime_compositor_xcb_backend_finalize(GObject* object) {
  ExpidusRuntimeCompositorXcbBackend* self = EXPIDUS_RUNTIME_COMPOSITOR_XCB_BACKEND(object);
  g_assert(self != NULL);

  G_OBJECT_CLASS(expidus_runtime_compositor_xcb_backend_parent_class)->finalize(object);
}

static void g_initable_interface_init(GInitableIface* interface) {
  interface->init = expidus_runtime_compositor_xcb_backend_initable_init;
}

static void expidus_runtime_compositor_xcb_backend_class_init(ExpidusRuntimeCompositorXcbBackendClass* klass) {
  ExpidusRuntimeCompositorBackendClass* backend_class = EXPIDUS_RUNTIME_COMPOSITOR_BACKEND_CLASS(klass);
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  backend_class->get_name = expidus_runtime_compositor_xcb_backend_get_name;

  object_class->constructed = expidus_runtime_compositor_xcb_backend_constructed;
  object_class->dispose = expidus_runtime_compositor_xcb_backend_dispose;
  object_class->finalize = expidus_runtime_compositor_xcb_backend_finalize;
}

static void expidus_runtime_compositor_xcb_backend_init(ExpidusRuntimeCompositorXcbBackend* self) {
  ExpidusRuntimeCompositorXcbBackendPrivate* priv = expidus_runtime_compositor_xcb_backend_get_instance_private(self);
  g_assert(priv != NULL);
  self->priv = priv;
}

ExpidusRuntimeCompositorBackend* expidus_runtime_compositor_xcb_backend_new(GCancellable* cancellable, GError** error) {
  return g_initable_new(EXPIDUS_RUNTIME_COMPOSITOR_TYPE_XCB_BACKEND, cancellable, error, NULL);
}
