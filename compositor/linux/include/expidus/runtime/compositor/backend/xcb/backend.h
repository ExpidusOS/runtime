#pragma once

#include <glib-object.h>
#include <expidus/runtime/compositor/backend.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define EXPIDUS_RUNTIME_COMPOSITOR_TYPE_XCB_BACKEND expidus_runtime_compositor_xcb_backend_get_type()
G_DECLARE_FINAL_TYPE(ExpidusRuntimeCompositorXcbBackend, expidus_runtime_compositor_xcb_backend, EXPIDUS_RUNTIME_COMPOSITOR, XCB_BACKEND, ExpidusRuntimeCompositorBackend);

struct _ExpidusRuntimeCompositorXcbBackend {
  ExpidusRuntimeCompositorBackend parent_instance;
  struct _ExpidusRuntimeCompositorXcbBackendPrivate* priv;
};

ExpidusRuntimeCompositorBackend* expidus_runtime_compositor_xcb_backend_new(GCancellable* cancellable, GError** error);

G_END_DECLS
