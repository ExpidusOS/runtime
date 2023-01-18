#include <expidus/runtime/compositor/rendering/scene.h>
#include <expidus/runtime/compositor/rendering/scene-layer.h>
#include "scene-priv.h"

G_DEFINE_TYPE_WITH_PRIVATE(ExpidusRuntimeCompositorScene, expidus_runtime_compositor_scene, G_TYPE_OBJECT);

static void expidus_runtime_compositor_scene_class_init(ExpidusRuntimeCompositorSceneClass* klass) {}

static void expidus_runtime_compositor_scene_init(ExpidusRuntimeCompositorScene* self) {
  ExpidusRuntimeCompositorScenePrivate* priv = expidus_runtime_compositor_scene_get_instance_private(self);
  g_assert(priv != NULL);

  self->priv = priv;
}

void expidus_runtime_compositor_scene_render(ExpidusRuntimeCompositorScene* self) {
  g_return_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_SCENE(self));

  for (GList* item = self->priv->layers; item != NULL; item = item->next) {
    ExpidusRuntimeCompositorSceneLayer* layer = EXPIDUS_RUNTIME_COMPOSITOR_SCENE_LAYER(item->data);
    g_assert(layer != NULL);
    expidus_runtime_compositor_scene_layer_render(layer);
  }
}
