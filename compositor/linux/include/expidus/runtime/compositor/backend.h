#pragma once

#include <glib-object.h>
#include <gio/gio.h>
#include <expidus/runtime/compositor/renderer.h>

G_BEGIN_DECLS

struct _ExpidusRuntimeCompositor;

#define EXPIDUS_RUNTIME_COMPOSITOR_TYPE_BACKEND expidus_runtime_compositor_backend_get_type()
G_DECLARE_DERIVABLE_TYPE(ExpidusRuntimeCompositorBackend, expidus_runtime_compositor_backend, EXPIDUS_RUNTIME_COMPOSITOR, BACKEND, GObject);

struct _ExpidusRuntimeCompositorBackendClass {
  GObjectClass parent_class;

  const gchar* (*get_name)(ExpidusRuntimeCompositorBackend* self);
  ExpidusRuntimeCompositorRenderer* (*get_renderer)(ExpidusRuntimeCompositorBackend* self);
  void (*run)(ExpidusRuntimeCompositorBackend* self);
  GList* (*get_outputs)(ExpidusRuntimeCompositorBackend* self);

  gpointer padding[12];
};

ExpidusRuntimeCompositorBackend* expidus_runtime_compositor_backend_new_auto(GCancellable* cancellable, GError** error);
ExpidusRuntimeCompositorBackend* expidus_runtime_compositor_backend_new(const gchar* name, GCancellable* cancellable, GError** error);

void expidus_runtime_compositor_backend_set_compositor(ExpidusRuntimeCompositorBackend* self, struct _ExpidusRuntimeCompositor* compositor);
struct _ExpidusRuntimeCompositor* expidus_runtime_compositor_backend_get_compositor(ExpidusRuntimeCompositorBackend* self);

const gchar* expidus_runtime_compositor_backend_get_name(ExpidusRuntimeCompositorBackend* self);
ExpidusRuntimeCompositorRenderer* expidus_runtime_compositor_backend_get_renderer(ExpidusRuntimeCompositorBackend* self);
void expidus_runtime_compositor_backend_run(ExpidusRuntimeCompositorBackend* self);

GList* expidus_runtime_compositor_backend_get_outputs(ExpidusRuntimeCompositorBackend* self);

G_END_DECLS
