#include <expidus/runtime/compositor/input.h>
#include "input-priv.h"

G_DEFINE_TYPE_WITH_PRIVATE(ExpidusRuntimeCompositorInput, expidus_runtime_compositor_input, G_TYPE_OBJECT);

enum {
  PROP_BACKEND = 1,
  N_PROPERTIES,
};

static GParamSpec* obj_properties[N_PROPERTIES] = { NULL };

static void expidus_runtime_compositor_input_constructed(GObject* object) {
  G_OBJECT_CLASS(expidus_runtime_compositor_input_parent_class)->constructed(object);

  ExpidusRuntimeCompositorInput* self = EXPIDUS_RUNTIME_COMPOSITOR_INPUT(object);
  g_debug("New input added: %s", expidus_runtime_compositor_input_get_name(self));
}

static void expidus_runtime_compositor_input_get_property(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec) {
  ExpidusRuntimeCompositorInput* self = EXPIDUS_RUNTIME_COMPOSITOR_INPUT(object);
  ExpidusRuntimeCompositorInputPrivate* priv = expidus_runtime_compositor_input_get_instance_private(self);

  switch (prop_id) {
    case PROP_BACKEND:
      g_value_set_object(value, priv->backend);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
      break;
  }
}

static void expidus_runtime_compositor_input_set_property(GObject* object, guint prop_id, const GValue* value, GParamSpec* pspec) {
  ExpidusRuntimeCompositorInput* self = EXPIDUS_RUNTIME_COMPOSITOR_INPUT(object);
  ExpidusRuntimeCompositorInputPrivate* priv = expidus_runtime_compositor_input_get_instance_private(self);

  switch (prop_id) {
    case PROP_BACKEND:
      priv->backend = EXPIDUS_RUNTIME_COMPOSITOR_BACKEND(g_value_dup_object(value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
      break;
  }
}

static void expidus_runtime_compositor_input_class_init(ExpidusRuntimeCompositorInputClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  object_class->constructed = expidus_runtime_compositor_input_constructed;
  object_class->get_property = expidus_runtime_compositor_input_get_property;
  object_class->set_property = expidus_runtime_compositor_input_set_property;

  obj_properties[PROP_BACKEND] = g_param_spec_object("backend", "Backend", "The display system backend instance", EXPIDUS_RUNTIME_COMPOSITOR_TYPE_BACKEND, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}

static void expidus_runtime_compositor_input_init(ExpidusRuntimeCompositorInput* self) {}

const gchar* expidus_runtime_compositor_input_get_name(ExpidusRuntimeCompositorInput* self) {
  g_return_val_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_INPUT(self), NULL);

  ExpidusRuntimeCompositorInputClass* klass = EXPIDUS_RUNTIME_COMPOSITOR_INPUT_GET_CLASS(self);
  g_assert(klass != NULL);

  g_return_val_if_fail(klass->get_name != NULL, NULL);
  return klass->get_name(self);
}

ExpidusRuntimeCompositorBackend* expidus_runtime_compositor_input_get_backend(ExpidusRuntimeCompositorInput* self) {
  g_return_val_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_INPUT(self), NULL);

  ExpidusRuntimeCompositorInputPrivate* priv = expidus_runtime_compositor_input_get_instance_private(self);
  g_assert(priv != NULL);
  return priv->backend;
}
