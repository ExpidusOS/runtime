#pragma once

#include <glib-object.h>
#include <flutter_embedder.h>
#include <expidus/runtime/compositor/backend.h>

G_BEGIN_DECLS

#define EXPIDUS_RUNTIME_COMPOSITOR_TYPE_OUTPUT expidus_runtime_compositor_output_get_type()
G_DECLARE_DERIVABLE_TYPE(ExpidusRuntimeCompositorOutput, expidus_runtime_compositor_output, EXPIDUS_RUNTIME_COMPOSITOR, OUTPUT, GObject);

struct _ExpidusRuntimeCompositorOutputClass {
  GObjectClass parent_class;

  FlutterEngineDisplay* (*get_engine)(ExpidusRuntimeCompositorOutput* self);

  gpointer padding[12];
};

ExpidusRuntimeCompositorBackend* expidus_runtime_compositor_output_get_backend(ExpidusRuntimeCompositorOutput* self);
FlutterEngineDisplay* expidus_runtime_compositor_output_get_engine(ExpidusRuntimeCompositorOutput* self);

G_END_DECLS
