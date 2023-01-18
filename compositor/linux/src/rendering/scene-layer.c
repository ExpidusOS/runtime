#include <expidus/runtime/compositor/rendering/scene-layer.h>

G_DEFINE_TYPE(ExpidusRuntimeCompositorSceneLayer, expidus_runtime_compositor_scene_layer, G_TYPE_OBJECT);

static void expidus_runtime_compositor_scene_layer_class_init(ExpidusRuntimeCompositorSceneLayerClass* klass) {}

static void expidus_runtime_compositor_scene_layer_init(ExpidusRuntimeCompositorSceneLayer* self) {}

void expidus_runtime_compositor_scene_layer_render(ExpidusRuntimeCompositorSceneLayer* self) {
  g_return_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_SCENE_LAYER(self));

  ExpidusRuntimeCompositorSceneLayerClass* klass = EXPIDUS_RUNTIME_COMPOSITOR_SCENE_LAYER_GET_CLASS(self);
  g_assert(klass != NULL);

  g_return_if_fail(klass->render != NULL);
  klass->render(self);
}
