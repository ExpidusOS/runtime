#pragma once

#include <glib-object.h>
#include <flutter_embedder.h>

G_BEGIN_DECLS

struct _ExpidusRuntimeCompositorBackend;

#define EXPIDUS_RUNTIME_COMPOSITOR_TYPE_RENDERER expidus_runtime_compositor_renderer_get_type()
G_DECLARE_DERIVABLE_TYPE(ExpidusRuntimeCompositorRenderer, expidus_runtime_compositor_renderer, EXPIDUS_RUNTIME_COMPOSITOR, RENDERER, GObject);

struct _ExpidusRuntimeCompositorRendererClass {
  GObjectClass parent_class;

  FlutterRendererConfig* (*get_config)(ExpidusRuntimeCompositorRenderer* self);
  FlutterCompositor* (*get_compositor)(ExpidusRuntimeCompositorRenderer* self);
  void (*wait_sync)(ExpidusRuntimeCompositorRenderer* self);

  gpointer padding[12];
};

FlutterRendererConfig* expidus_runtime_compositor_renderer_get_config(ExpidusRuntimeCompositorRenderer* self);
FlutterCompositor* expidus_runtime_compositor_renderer_get_compositor(ExpidusRuntimeCompositorRenderer* self);
void expidus_runtime_compositor_renderer_wait_sync(ExpidusRuntimeCompositorRenderer* self);
struct _ExpidusRuntimeCompositorBackend* expidus_runtime_compositor_renderer_get_backend(ExpidusRuntimeCompositorRenderer* self);

G_END_DECLS
