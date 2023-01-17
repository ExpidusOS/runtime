#include <expidus/runtime/compositor/renderer/egl/renderer.h>
#include "renderer-priv.h"

G_DEFINE_TYPE_WITH_PRIVATE(ExpidusRuntimeCompositorEglRenderer, expidus_runtime_compositor_egl_renderer, EXPIDUS_RUNTIME_COMPOSITOR_TYPE_RENDERER);

enum {
  PROP_DISPLAY = 1,
  PROP_CONTEXT,
  N_PROPERTIES,
};

static GParamSpec* obj_properties[N_PROPERTIES] = { NULL };

static FlutterRendererConfig* expidus_runtime_compositor_egl_renderer_get_config(ExpidusRuntimeCompositorRenderer* renderer) {
  ExpidusRuntimeCompositorEglRenderer* self = EXPIDUS_RUNTIME_COMPOSITOR_EGL_RENDERER(renderer);
  return &self->priv->config;
}

static void expidus_runtime_compositor_egl_renderer_constructed(GObject* object) {
  G_OBJECT_CLASS(expidus_runtime_compositor_egl_renderer_parent_class)->constructed(object);

  ExpidusRuntimeCompositorEglRenderer* self = EXPIDUS_RUNTIME_COMPOSITOR_EGL_RENDERER(object);
  self->priv->config.type = kOpenGL;
}

static void expidus_runtime_compositor_egl_renderer_get_property(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec) {
  ExpidusRuntimeCompositorEglRenderer* self = EXPIDUS_RUNTIME_COMPOSITOR_EGL_RENDERER(object);

  switch (prop_id) {
    case PROP_DISPLAY:
      g_value_set_pointer(value, self->priv->display);
      break;
    case PROP_CONTEXT:
      g_value_set_pointer(value, self->priv->context);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
      break;
  }
}

static void expidus_runtime_compositor_egl_renderer_set_property(GObject* object, guint prop_id, const GValue* value, GParamSpec* pspec) {
  ExpidusRuntimeCompositorEglRenderer* self = EXPIDUS_RUNTIME_COMPOSITOR_EGL_RENDERER(object);

  switch (prop_id) {
    case PROP_DISPLAY:
      self->priv->display = g_value_get_pointer(value);
      break;
    case PROP_CONTEXT:
      self->priv->context = g_value_get_pointer(value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
      break;
  }
}

static void expidus_runtime_compositor_egl_renderer_class_init(ExpidusRuntimeCompositorEglRendererClass* klass) {
  ExpidusRuntimeCompositorRendererClass* renderer_class = EXPIDUS_RUNTIME_COMPOSITOR_RENDERER_CLASS(klass);
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  renderer_class->get_config = expidus_runtime_compositor_egl_renderer_get_config;

  object_class->constructed = expidus_runtime_compositor_egl_renderer_constructed;
  object_class->get_property = expidus_runtime_compositor_egl_renderer_get_property;
  object_class->set_property = expidus_runtime_compositor_egl_renderer_set_property;

  obj_properties[PROP_DISPLAY] = g_param_spec_pointer("display", "EGL Display", "The EGL display being used", G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  obj_properties[PROP_CONTEXT] = g_param_spec_pointer("context", "EGL Context", "The EGL context being used", G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}

static void expidus_runtime_compositor_egl_renderer_init(ExpidusRuntimeCompositorEglRenderer* self) {
  ExpidusRuntimeCompositorEglRendererPrivate* priv = expidus_runtime_compositor_egl_renderer_get_instance_private(self);
  g_assert(priv != NULL);
  self->priv = priv;
  self->priv->config.open_gl.struct_size = sizeof (FlutterOpenGLRendererConfig);
}

ExpidusRuntimeCompositorRenderer* expidus_runtime_compositor_egl_renderer_new(EGLDisplay display, EGLContext context) {
  return g_object_new(EXPIDUS_RUNTIME_COMPOSITOR_TYPE_EGL_RENDERER, "display", display, "context", context);
}
