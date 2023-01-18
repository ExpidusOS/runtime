#include <expidus/runtime/compositor/rendering/scene.h>
#include <expidus/runtime/compositor/rendering/scene-layer.h>
#include "scene-priv.h"

G_DEFINE_TYPE_WITH_PRIVATE(ExpidusRuntimeCompositorScene, expidus_runtime_compositor_scene, G_TYPE_OBJECT);

static void expidus_runtime_compositor_scene_constructed(GObject* object) {
  G_OBJECT_CLASS(expidus_runtime_compositor_scene_parent_class)->constructed(object);

  ExpidusRuntimeCompositorScene* self = EXPIDUS_RUNTIME_COMPOSITOR_SCENE(object);
  g_assert(self != NULL);

  if (pthread_mutex_init(&self->priv->mutex, NULL) != 0) {
    g_error("Failed to initialize mutex for scene");
  }
}

static void expidus_runtime_compositor_scene_class_init(ExpidusRuntimeCompositorSceneClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  object_class->constructed = expidus_runtime_compositor_scene_constructed;
}

static void expidus_runtime_compositor_scene_init(ExpidusRuntimeCompositorScene* self) {
  ExpidusRuntimeCompositorScenePrivate* priv = expidus_runtime_compositor_scene_get_instance_private(self);
  g_assert(priv != NULL);

  self->priv = priv;
}

ExpidusRuntimeCompositorScene* expidus_runtime_compositor_scene_new() {
  return g_object_new(EXPIDUS_RUNTIME_COMPOSITOR_TYPE_SCENE, NULL);
}

guint expidus_runtime_compositor_scene_add_layer(ExpidusRuntimeCompositorScene* self, ExpidusRuntimeCompositorSceneLayer* layer) {
  g_return_val_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_SCENE(self), 0);

  pthread_mutex_lock(&self->priv->mutex);

  self->priv->layers = g_list_append(self->priv->layers, layer);
  guint i = g_list_length(self->priv->layers);

  pthread_mutex_unlock(&self->priv->mutex);
  return i;
}

void expidus_runtime_compositor_scene_render(ExpidusRuntimeCompositorScene* self) {
  g_return_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_SCENE(self));

  pthread_mutex_lock(&self->priv->mutex);

  for (GList* item = self->priv->layers; item != NULL; item = item->next) {
    ExpidusRuntimeCompositorSceneLayer* layer = EXPIDUS_RUNTIME_COMPOSITOR_SCENE_LAYER(item->data);
    g_assert(layer != NULL);
    expidus_runtime_compositor_scene_layer_render(layer);
  }

  pthread_mutex_unlock(&self->priv->mutex);
}

void expidus_runtime_compositor_scene_clean(ExpidusRuntimeCompositorScene* self) {
  g_return_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_SCENE(self));

  pthread_mutex_lock(&self->priv->mutex);

  g_list_free_full(self->priv->layers, g_object_unref);
  self->priv->layers = NULL;

  pthread_mutex_unlock(&self->priv->mutex);
}
