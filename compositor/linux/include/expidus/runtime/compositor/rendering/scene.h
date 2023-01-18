#pragma once

#include <glib-object.h>
#include <expidus/runtime/compositor/rendering/scene-layer.h>

G_BEGIN_DECLS

#define EXPIDUS_RUNTIME_COMPOSITOR_TYPE_SCENE expidus_runtime_compositor_scene_get_type()
G_DECLARE_FINAL_TYPE(ExpidusRuntimeCompositorScene, expidus_runtime_compositor_scene, EXPIDUS_RUNTIME_COMPOSITOR, SCENE, GObject);

struct _ExpidusRuntimeCompositorScene {
  GObject parent_instance;
  struct _ExpidusRuntimeCompositorScenePrivate* priv;
};

ExpidusRuntimeCompositorScene* expidus_runtime_compositor_scene_new();
guint expidus_runtime_compositor_scene_add_layer(ExpidusRuntimeCompositorScene* self, ExpidusRuntimeCompositorSceneLayer* layer);
void expidus_runtime_compositor_scene_render(ExpidusRuntimeCompositorScene* self);
void expidus_runtime_compositor_scene_clean(ExpidusRuntimeCompositorScene* self);

G_END_DECLS
