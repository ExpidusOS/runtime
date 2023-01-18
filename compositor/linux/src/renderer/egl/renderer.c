#include <expidus/runtime/compositor/renderer/egl/renderer.h>
#include <expidus/runtime/compositor.h>
#include "renderer-priv.h"

G_DEFINE_TYPE_WITH_PRIVATE(ExpidusRuntimeCompositorEglRenderer, expidus_runtime_compositor_egl_renderer, EXPIDUS_RUNTIME_COMPOSITOR_TYPE_RENDERER);

enum {
  PROP_DISPLAY = 1,
  PROP_CONTEXT,
  N_PROPERTIES,
};

static GParamSpec* obj_properties[N_PROPERTIES] = { NULL };

static bool expidus_runtime_compositor_egl_renderer_make_current(void* user_data) {
  ExpidusRuntimeCompositor* compositor = EXPIDUS_RUNTIME_COMPOSITOR(user_data);
  g_assert(compositor != NULL);

  ExpidusRuntimeCompositorBackend* backend = expidus_runtime_compositor_get_backend(compositor);
  g_assert(backend != NULL);

  ExpidusRuntimeCompositorRenderer* renderer = expidus_runtime_compositor_backend_get_renderer(backend);
  g_assert(renderer != NULL);

  ExpidusRuntimeCompositorEglRenderer* self = EXPIDUS_RUNTIME_COMPOSITOR_EGL_RENDERER(renderer);
  g_assert(self != NULL);

  eglMakeCurrent(self->priv->display, EGL_NO_SURFACE, EGL_NO_SURFACE, self->priv->flutter_context);
  return TRUE;
}

static bool expidus_runtime_compositor_egl_renderer_clear_current(void* user_data) {
  ExpidusRuntimeCompositor* compositor = EXPIDUS_RUNTIME_COMPOSITOR(user_data);
  g_assert(compositor != NULL);

  ExpidusRuntimeCompositorBackend* backend = expidus_runtime_compositor_get_backend(compositor);
  g_assert(backend != NULL);

  ExpidusRuntimeCompositorRenderer* renderer = expidus_runtime_compositor_backend_get_renderer(backend);
  g_assert(renderer != NULL);

  ExpidusRuntimeCompositorEglRenderer* self = EXPIDUS_RUNTIME_COMPOSITOR_EGL_RENDERER(renderer);
  g_assert(self != NULL);

  eglMakeCurrent(self->priv->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
  return TRUE;
}

static bool expidus_runtime_compositor_egl_renderer_make_resource_current(void* user_data) {
  ExpidusRuntimeCompositor* compositor = EXPIDUS_RUNTIME_COMPOSITOR(user_data);
  g_assert(compositor != NULL);

  ExpidusRuntimeCompositorBackend* backend = expidus_runtime_compositor_get_backend(compositor);
  g_assert(backend != NULL);

  ExpidusRuntimeCompositorRenderer* renderer = expidus_runtime_compositor_backend_get_renderer(backend);
  g_assert(renderer != NULL);

  ExpidusRuntimeCompositorEglRenderer* self = EXPIDUS_RUNTIME_COMPOSITOR_EGL_RENDERER(renderer);
  g_assert(self != NULL);

  eglMakeCurrent(self->priv->display, EGL_NO_SURFACE, EGL_NO_SURFACE, self->priv->flutter_resource_context);
  return TRUE;
}

static void* expidus_runtime_compositor_egl_renderer_gl_proc_resolver(void* user_data, const char* name) {
  return eglGetProcAddress(name);
}

static uint32_t expidus_runtime_compositor_egl_renderer_fbo_with_frame_info_callback(void* user_data, const FlutterFrameInfo* frame_info) {
  return 0;
}

static bool expidus_runtime_compositor_egl_renderer_present_with_info(void* user_data, const FlutterPresentInfo* present_info) {
  ExpidusRuntimeCompositor* compositor = EXPIDUS_RUNTIME_COMPOSITOR(user_data);
  g_assert(compositor != NULL);

  ExpidusRuntimeCompositorBackend* backend = expidus_runtime_compositor_get_backend(compositor);
  g_assert(backend != NULL);

  ExpidusRuntimeCompositorRenderer* renderer = expidus_runtime_compositor_backend_get_renderer(backend);
  g_assert(renderer != NULL);

  ExpidusRuntimeCompositorEglRenderer* self = EXPIDUS_RUNTIME_COMPOSITOR_EGL_RENDERER(renderer);
  g_assert(self != NULL);
  return TRUE;
}

static FlutterRendererConfig* expidus_runtime_compositor_egl_renderer_get_config(ExpidusRuntimeCompositorRenderer* renderer) {
  ExpidusRuntimeCompositorEglRenderer* self = EXPIDUS_RUNTIME_COMPOSITOR_EGL_RENDERER(renderer);
  return &self->priv->config;
}

static FlutterCompositor* expidus_runtime_compositor_egl_renderer_get_compositor(ExpidusRuntimeCompositorRenderer* renderer) {
  ExpidusRuntimeCompositorEglRenderer* self = EXPIDUS_RUNTIME_COMPOSITOR_EGL_RENDERER(renderer);
  return &self->priv->compositor;
}

static void expidus_runtime_compositor_egl_renderer_constructed(GObject* object) {
  G_OBJECT_CLASS(expidus_runtime_compositor_egl_renderer_parent_class)->constructed(object);

  ExpidusRuntimeCompositorEglRenderer* self = EXPIDUS_RUNTIME_COMPOSITOR_EGL_RENDERER(object);

  const char* exts = eglQueryString(self->priv->display, EGL_EXTENSIONS);

  size_t atti = 0;
  EGLint attribs[5];

  attribs[atti++] = EGL_CONTEXT_CLIENT_VERSION;
  attribs[atti++] = 2;

  if (strstr(exts, "EGL_IMG_context_priority") != NULL) {
    attribs[atti++] = EGL_CONTEXT_PRIORITY_LEVEL_IMG;
    attribs[atti++] = EGL_CONTEXT_PRIORITY_HIGH_IMG;
  }

  attribs[atti++] = EGL_NONE;

  self->priv->flutter_context = eglCreateContext(self->priv->display, EGL_NO_CONFIG_KHR, self->priv->context, attribs);
  g_assert(self->priv->flutter_context != NULL);

  self->priv->flutter_resource_context = eglCreateContext(self->priv->display, EGL_NO_CONFIG_KHR, self->priv->flutter_context, attribs);
  g_assert(self->priv->flutter_resource_context != NULL);
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
  renderer_class->get_compositor = expidus_runtime_compositor_egl_renderer_get_compositor;

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

  self->priv->config.type = kOpenGL;
  self->priv->config.open_gl.struct_size = sizeof (FlutterOpenGLRendererConfig);
  self->priv->config.open_gl.make_current = expidus_runtime_compositor_egl_renderer_make_current;
  self->priv->config.open_gl.clear_current = expidus_runtime_compositor_egl_renderer_clear_current;
  self->priv->config.open_gl.make_resource_current = expidus_runtime_compositor_egl_renderer_make_resource_current;
  self->priv->config.open_gl.fbo_reset_after_present = TRUE;
  self->priv->config.open_gl.gl_proc_resolver = expidus_runtime_compositor_egl_renderer_gl_proc_resolver;
  self->priv->config.open_gl.fbo_with_frame_info_callback = expidus_runtime_compositor_egl_renderer_fbo_with_frame_info_callback;
  self->priv->config.open_gl.present_with_info = expidus_runtime_compositor_egl_renderer_present_with_info;

  self->priv->compositor.struct_size = sizeof (FlutterCompositor);
  self->priv->compositor.user_data = self;
  self->priv->compositor.avoid_backing_store_cache = TRUE;
}

ExpidusRuntimeCompositorRenderer* expidus_runtime_compositor_egl_renderer_new(EGLDisplay display, EGLContext context) {
  return g_object_new(EXPIDUS_RUNTIME_COMPOSITOR_TYPE_EGL_RENDERER, "display", display, "context", context);
}
