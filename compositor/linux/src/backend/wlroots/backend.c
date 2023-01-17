#include <expidus/runtime/compositor/backend/wlroots/backend.h>
#include <gio/gio.h>
#include <wlr/util/log.h>
#include "backend-priv.h"

static void g_initable_interface_init(GInitableIface* interface);

G_DEFINE_TYPE_WITH_CODE(ExpidusRuntimeCompositorWlrootsBackend, expidus_runtime_compositor_wlroots_backend, EXPIDUS_RUNTIME_COMPOSITOR_TYPE_BACKEND,
    G_ADD_PRIVATE(ExpidusRuntimeCompositorWlrootsBackend)
    G_IMPLEMENT_INTERFACE(G_TYPE_INITABLE, g_initable_interface_init));

static gboolean expidus_runtime_compositor_wlroots_backend_initable_init(GInitable* initable, GCancellable* cancellable, GError** error) {
  ExpidusRuntimeCompositorWlrootsBackend* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_BACKEND(initable);
  g_assert(self != NULL);
  return TRUE;
}

static const gchar* expidus_runtime_compositor_wlroots_backend_get_name(ExpidusRuntimeCompositorBackend* backend) {
  return "wlroots";
}

static void expidus_runtime_compositor_wlroots_backend_run(ExpidusRuntimeCompositorBackend* backend) {
  ExpidusRuntimeCompositorWlrootsBackend* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_BACKEND(backend);
  g_assert(self != NULL);

  wl_display_run(self->priv->display);
}

static void expidus_runtime_compositor_wlroots_backend_constructed(GObject* object) {
  G_OBJECT_CLASS(expidus_runtime_compositor_wlroots_backend_parent_class)->constructed(object);

  ExpidusRuntimeCompositorWlrootsBackend* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_BACKEND(object);
  g_assert(self != NULL);

  self->priv->display = wl_display_create();
  self->priv->event_loop = wl_display_get_event_loop(self->priv->display);
  self->priv->backend = wlr_backend_autocreate(self->priv->display);
}

static void expidus_runtime_compositor_wlroots_backend_dispose(GObject* object) {
  ExpidusRuntimeCompositorWlrootsBackend* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_BACKEND(object);
  g_assert(self != NULL);

  g_clear_pointer(&self->priv->backend, wlr_backend_destroy);
  g_clear_pointer(&self->priv->display, wl_display_destroy);

  G_OBJECT_CLASS(expidus_runtime_compositor_wlroots_backend_parent_class)->dispose(object);
}

static void expidus_runtime_compositor_wlroots_backend_finalize(GObject* object) {
  ExpidusRuntimeCompositorWlrootsBackend* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_BACKEND(object);
  g_assert(self != NULL);

  G_OBJECT_CLASS(expidus_runtime_compositor_wlroots_backend_parent_class)->finalize(object);
}

static void g_initable_interface_init(GInitableIface* interface) {
  interface->init = expidus_runtime_compositor_wlroots_backend_initable_init;
}

static void expidus_runtime_compositor_wlroots_backend_class_init(ExpidusRuntimeCompositorWlrootsBackendClass* klass) {
  ExpidusRuntimeCompositorBackendClass* backend_class = EXPIDUS_RUNTIME_COMPOSITOR_BACKEND_CLASS(klass);
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  backend_class->get_name = expidus_runtime_compositor_wlroots_backend_get_name;
  backend_class->run = expidus_runtime_compositor_wlroots_backend_run;

  object_class->constructed = expidus_runtime_compositor_wlroots_backend_constructed;
  object_class->dispose = expidus_runtime_compositor_wlroots_backend_dispose;
  object_class->finalize = expidus_runtime_compositor_wlroots_backend_finalize;
}

static void expidus_runtime_compositor_wlroots_backend_init(ExpidusRuntimeCompositorWlrootsBackend* self) {
  ExpidusRuntimeCompositorWlrootsBackendPrivate* priv = expidus_runtime_compositor_wlroots_backend_get_instance_private(self);
  g_assert(priv != NULL);
  self->priv = priv;

  wlr_log_init(WLR_DEBUG, NULL);
}

ExpidusRuntimeCompositorBackend* expidus_runtime_compositor_wlroots_backend_new(GCancellable* cancellable, GError** error) {
  return g_initable_new(EXPIDUS_RUNTIME_COMPOSITOR_TYPE_WLROOTS_BACKEND, cancellable, error, NULL);
}
