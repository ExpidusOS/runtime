#pragma once

#include <glib-object.h>
#include <expidus/runtime/compositor/backend.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define EXPIDUS_RUNTIME_COMPOSITOR_TYPE_WLROOTS_BACKEND expidus_runtime_compositor_wlroots_backend_get_type()
G_DECLARE_FINAL_TYPE(ExpidusRuntimeCompositorWlrootsBackend, expidus_runtime_compositor_wlroots_backend, EXPIDUS_RUNTIME_COMPOSITOR, WLROOTS_BACKEND, ExpidusRuntimeCompositorBackend);

struct _ExpidusRuntimeCompositorWlrootsBackend {
  ExpidusRuntimeCompositorBackend parent_instance;
  struct _ExpidusRuntimeCompositorWlrootsBackendPrivate* priv;
};

ExpidusRuntimeCompositorBackend* expidus_runtime_compositor_wlroots_backend_new(GCancellable* cancellable, GError** error);

G_END_DECLS
