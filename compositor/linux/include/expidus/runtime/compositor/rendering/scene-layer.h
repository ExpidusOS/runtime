#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

#define EXPIDUS_RUNTIME_COMPOSITOR_TYPE_SCENE_LAYER expidus_runtime_compositor_scene_layer_get_type()
G_DECLARE_DERIVABLE_TYPE(ExpidusRuntimeCompositorSceneLayer, expidus_runtime_compositor_scene_layer, EXPIDUS_RUNTIME_COMPOSITOR, SCENE_LAYER, GObject);

struct _ExpidusRuntimeCompositorSceneLayerClass {
  GObjectClass parent_class;

  void (*render)(ExpidusRuntimeCompositorSceneLayer* self);

  gpointer padding[12];
};

void expidus_runtime_compositor_scene_layer_render(ExpidusRuntimeCompositorSceneLayer* self);

G_END_DECLS
