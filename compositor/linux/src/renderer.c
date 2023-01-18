#include <expidus/runtime/compositor/renderer.h>
#include <glib.h>
#include "renderer-priv.h"

G_DEFINE_TYPE_WITH_PRIVATE(ExpidusRuntimeCompositorRenderer, expidus_runtime_compositor_renderer, G_TYPE_OBJECT);

enum {
  PROP_BACKEND = 1,
  N_PROPERTIES,
};

static GParamSpec* obj_properties[N_PROPERTIES] = { NULL };

static void expidus_runtime_compositor_renderer_dispose(GObject* object) {
  ExpidusRuntimeCompositorRenderer* self = EXPIDUS_RUNTIME_COMPOSITOR_RENDERER(object);
  ExpidusRuntimeCompositorRendererPrivate* priv = expidus_runtime_compositor_renderer_get_instance_private(self);

  g_clear_object(&priv->backend);

  G_OBJECT_CLASS(expidus_runtime_compositor_renderer_parent_class)->dispose(object);
}

static void expidus_runtime_compositor_renderer_get_property(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec) {
  ExpidusRuntimeCompositorRenderer* self = EXPIDUS_RUNTIME_COMPOSITOR_RENDERER(object);
  ExpidusRuntimeCompositorRendererPrivate* priv = expidus_runtime_compositor_renderer_get_instance_private(self);

  switch (prop_id) {
    case PROP_BACKEND:
      g_value_set_object(value, priv->backend);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
      break;
  }
}

static void expidus_runtime_compositor_renderer_set_property(GObject* object, guint prop_id, const GValue* value, GParamSpec* pspec) {
  ExpidusRuntimeCompositorRenderer* self = EXPIDUS_RUNTIME_COMPOSITOR_RENDERER(object);
  ExpidusRuntimeCompositorRendererPrivate* priv = expidus_runtime_compositor_renderer_get_instance_private(self);

  switch (prop_id) {
    case PROP_BACKEND:
      priv->backend = EXPIDUS_RUNTIME_COMPOSITOR_BACKEND(g_value_dup_object(value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
      break;
  }
}

static void expidus_runtime_compositor_renderer_class_init(ExpidusRuntimeCompositorRendererClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  object_class->dispose = expidus_runtime_compositor_renderer_dispose;
  object_class->get_property = expidus_runtime_compositor_renderer_get_property;
  object_class->set_property = expidus_runtime_compositor_renderer_set_property;

  obj_properties[PROP_BACKEND] = g_param_spec_object("backend", "Backend", "The display system backend instance", EXPIDUS_RUNTIME_COMPOSITOR_TYPE_BACKEND, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}

static void expidus_runtime_compositor_renderer_init(ExpidusRuntimeCompositorRenderer* self) {}

FlutterRendererConfig* expidus_runtime_compositor_renderer_get_config(ExpidusRuntimeCompositorRenderer* self) {
  g_return_val_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_RENDERER(self), NULL);

  ExpidusRuntimeCompositorRendererClass* klass = EXPIDUS_RUNTIME_COMPOSITOR_RENDERER_GET_CLASS(self);
  g_assert(klass != NULL);

  g_return_val_if_fail(klass->get_config != NULL, NULL);
  return klass->get_config(self);
}

FlutterCompositor* expidus_runtime_compositor_renderer_get_compositor(ExpidusRuntimeCompositorRenderer* self) {
  g_return_val_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_RENDERER(self), NULL);

  ExpidusRuntimeCompositorRendererClass* klass = EXPIDUS_RUNTIME_COMPOSITOR_RENDERER_GET_CLASS(self);
  g_assert(klass != NULL);

  if (klass->get_compositor != NULL) return klass->get_compositor(self);
  return NULL;
}

void expidus_runtime_compositor_renderer_wait_sync(ExpidusRuntimeCompositorRenderer* self) {
  g_return_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_RENDERER(self));

  ExpidusRuntimeCompositorRendererClass* klass = EXPIDUS_RUNTIME_COMPOSITOR_RENDERER_GET_CLASS(self);
  g_assert(klass != NULL);

  if (klass->wait_sync != NULL) klass->wait_sync(self);
}

void expidus_runtime_compositor_renderer_render(ExpidusRuntimeCompositorRenderer* self) {
  g_return_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_RENDERER(self));

  ExpidusRuntimeCompositorRendererClass* klass = EXPIDUS_RUNTIME_COMPOSITOR_RENDERER_GET_CLASS(self);
  g_assert(klass != NULL);

  g_return_if_fail(klass->render != NULL);
  klass->render(self);
}

struct _ExpidusRuntimeCompositorBackend* expidus_runtime_compositor_renderer_get_backend(ExpidusRuntimeCompositorRenderer* self) {
  g_return_val_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_RENDERER(self), NULL);

  ExpidusRuntimeCompositorRendererPrivate* priv = expidus_runtime_compositor_renderer_get_instance_private(self);
  g_assert(priv != NULL);
  return priv->backend;
}
