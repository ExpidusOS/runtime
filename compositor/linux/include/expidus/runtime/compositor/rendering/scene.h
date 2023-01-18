#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

#define EXPIDUS_RUNTIME_COMPOSITOR_TYPE_SCENE expidus_runtime_compositor_scene_get_type()
G_DECLARE_FINAL_TYPE(ExpidusRuntimeCompositorScene, expidus_runtime_compositor_scene, EXPIDUS_RUNTIME_COMPOSITOR, SCENE, GObject);

struct _ExpidusRuntimeCompositorScene {
  GObject parent_instance;
  struct _ExpidusRuntimeCompositorScenePrivate* priv;
};

void expidus_runtime_compositor_scene_render(ExpidusRuntimeCompositorScene* self);

G_END_DECLS
