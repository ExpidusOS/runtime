#include <expidus/runtime/compositor/rendering/scene-layer.h>
#include "scene-layer-priv.h"

G_DEFINE_TYPE_WITH_PRIVATE(ExpidusRuntimeCompositorSceneLayer, expidus_runtime_compositor_scene_layer, G_TYPE_OBJECT);

enum {
  PROP_LAYER = 1,
  N_PROPERTIES,
};

static GParamSpec* obj_properties[N_PROPERTIES] = { NULL };

static void expidus_runtime_compositor_scene_layer_get_property(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec) {
  ExpidusRuntimeCompositorSceneLayer* self = EXPIDUS_RUNTIME_COMPOSITOR_SCENE_LAYER(object);
  ExpidusRuntimeCompositorSceneLayerPrivate* priv = expidus_runtime_compositor_scene_layer_get_instance_private(self);

  switch (prop_id) {
    case PROP_LAYER:
      g_value_set_pointer(value, priv->layer);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
      break;
  }
}

static void expidus_runtime_compositor_scene_layer_set_property(GObject* object, guint prop_id, const GValue* value, GParamSpec* pspec) {
  ExpidusRuntimeCompositorSceneLayer* self = EXPIDUS_RUNTIME_COMPOSITOR_SCENE_LAYER(object);
  ExpidusRuntimeCompositorSceneLayerPrivate* priv = expidus_runtime_compositor_scene_layer_get_instance_private(self);

  switch (prop_id) {
    case PROP_LAYER:
      priv->layer = g_value_get_pointer(value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
      break;
  }
}

static void expidus_runtime_compositor_scene_layer_class_init(ExpidusRuntimeCompositorSceneLayerClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  object_class->get_property = expidus_runtime_compositor_scene_layer_get_property;
  object_class->set_property = expidus_runtime_compositor_scene_layer_set_property;

  obj_properties[PROP_LAYER] = g_param_spec_pointer("layer", "Layer", "A Flutter backing-store layer", G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}

static void expidus_runtime_compositor_scene_layer_init(ExpidusRuntimeCompositorSceneLayer* self) {}

void expidus_runtime_compositor_scene_layer_render(ExpidusRuntimeCompositorSceneLayer* self) {
  g_return_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_SCENE_LAYER(self));

  ExpidusRuntimeCompositorSceneLayerClass* klass = EXPIDUS_RUNTIME_COMPOSITOR_SCENE_LAYER_GET_CLASS(self);
  g_assert(klass != NULL);

  g_return_if_fail(klass->render != NULL);
  klass->render(self);
}
