#pragma once

#include <expidus/runtime/compositor/backend.h>
#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define EXPIDUS_RUNTIME_TYPE_COMPOSITOR expidus_runtime_compositor_get_type()
G_DECLARE_FINAL_TYPE(ExpidusRuntimeCompositor, expidus_runtime_compositor, EXPIDUS_RUNTIME, COMPOSITOR, GApplication);

struct _ExpidusRuntimeCompositor {
  GApplication parent_instance;
  struct _ExpidusRuntimeCompositorPrivate* priv;
};

ExpidusRuntimeCompositor* expidus_runtime_compositor_new(const gchar* application_id, const gchar* binary_name);
ExpidusRuntimeCompositor* expidus_runtime_compositor_new_with_backend(ExpidusRuntimeCompositorBackend* backend, const gchar* application_id, const gchar* binary_name);

ExpidusRuntimeCompositorBackend* expidus_runtime_compositor_get_backend(ExpidusRuntimeCompositor* self);
FlutterEngine* expidus_runtime_compositor_get_engine(ExpidusRuntimeCompositor* self);

FlutterEngineResult expidus_runtime_compositor_backend_display_update(ExpidusRuntimeCompositor* self, FlutterEngineDisplaysUpdateType update_type);

G_END_DECLS
