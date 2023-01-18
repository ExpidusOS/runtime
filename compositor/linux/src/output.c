#include <expidus/runtime/compositor/output.h>
#include "output-priv.h"

G_DEFINE_TYPE_WITH_PRIVATE(ExpidusRuntimeCompositorOutput, expidus_runtime_compositor_output, G_TYPE_OBJECT);

enum {
  PROP_BACKEND = 1,
  N_PROPERTIES,
};

static GParamSpec* obj_properties[N_PROPERTIES] = { NULL };

static void expidus_runtime_compositor_output_get_property(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec) {
  ExpidusRuntimeCompositorOutput* self = EXPIDUS_RUNTIME_COMPOSITOR_OUTPUT(object);
  ExpidusRuntimeCompositorOutputPrivate* priv = expidus_runtime_compositor_output_get_instance_private(self);

  switch (prop_id) {
    case PROP_BACKEND:
      g_value_set_object(value, priv->backend);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
      break;
  }
}

static void expidus_runtime_compositor_output_set_property(GObject* object, guint prop_id, const GValue* value, GParamSpec* pspec) {
  ExpidusRuntimeCompositorOutput* self = EXPIDUS_RUNTIME_COMPOSITOR_OUTPUT(object);
  ExpidusRuntimeCompositorOutputPrivate* priv = expidus_runtime_compositor_output_get_instance_private(self);

  switch (prop_id) {
    case PROP_BACKEND:
      priv->backend = EXPIDUS_RUNTIME_COMPOSITOR_BACKEND(g_value_dup_object(value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
      break;
  }
}

static void expidus_runtime_compositor_output_class_init(ExpidusRuntimeCompositorOutputClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  object_class->get_property = expidus_runtime_compositor_output_get_property;
  object_class->set_property = expidus_runtime_compositor_output_set_property;

  obj_properties[PROP_BACKEND] = g_param_spec_object("backend", "Backend", "The display system backend instance", EXPIDUS_RUNTIME_COMPOSITOR_TYPE_BACKEND, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}

static void expidus_runtime_compositor_output_init(ExpidusRuntimeCompositorOutput* self) {}

ExpidusRuntimeCompositorBackend* expidus_runtime_compositor_output_get_backend(ExpidusRuntimeCompositorOutput* self) {
  g_return_val_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_OUTPUT(self), NULL);

  ExpidusRuntimeCompositorOutputPrivate* priv = expidus_runtime_compositor_output_get_instance_private(self);
  g_assert(priv != NULL);

  return priv->backend;
}

FlutterEngineDisplay* expidus_runtime_compositor_output_get_engine(ExpidusRuntimeCompositorOutput* self) {
  g_return_val_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_OUTPUT(self), NULL);

  ExpidusRuntimeCompositorOutputClass* klass = EXPIDUS_RUNTIME_COMPOSITOR_OUTPUT_GET_CLASS(self);
  g_assert(klass != NULL);

  g_return_val_if_fail(klass->get_engine != NULL, NULL);
  return klass->get_engine(self);
}

FlutterWindowMetricsEvent expidus_runtime_compositor_output_get_window_metrics(ExpidusRuntimeCompositorOutput* self) {
  g_assert(EXPIDUS_RUNTIME_COMPOSITOR_IS_OUTPUT(self));

  ExpidusRuntimeCompositorOutputClass* klass = EXPIDUS_RUNTIME_COMPOSITOR_OUTPUT_GET_CLASS(self);
  g_assert(klass != NULL);

  g_assert(klass->get_window_metrics != NULL);
  return klass->get_window_metrics(self);
}
