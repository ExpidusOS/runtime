#pragma once

#include <glib-object.h>
#include <flutter_embedder.h>

G_BEGIN_DECLS

#define EXPIDUS_RUNTIME_COMPOSITOR_TYPE_RENDERER expidus_runtime_compositor_renderer_get_type()
G_DECLARE_DERIVABLE_TYPE(ExpidusRuntimeCompositorRenderer, expidus_runtime_compositor_renderer, EXPIDUS_RUNTIME_COMPOSITOR, RENDERER, GObject);

struct _ExpidusRuntimeCompositorRendererClass {
  GObjectClass parent_class;

  FlutterRendererConfig* (*get_config)(ExpidusRuntimeCompositorRenderer* self);

  gpointer padding[12];
};

FlutterRendererConfig* expidus_runtime_compositor_renderer_get_config(ExpidusRuntimeCompositorRenderer* self);

G_END_DECLS
