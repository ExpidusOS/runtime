#include <expidus/runtime/compositor/renderer/egl/renderer.h>
#include <expidus/runtime/compositor/renderer/egl/scene-layer.h>
#include <expidus/runtime/compositor/renderer/egl/shader.h>
#include <expidus/runtime/compositor.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include "renderer-priv.h"
#include "shader-priv.h"

#define GL_BGRA_EXT 0x80E1

G_DEFINE_TYPE_WITH_PRIVATE(ExpidusRuntimeCompositorEglRenderer, expidus_runtime_compositor_egl_renderer, EXPIDUS_RUNTIME_COMPOSITOR_TYPE_RENDERER);

enum {
  PROP_DISPLAY = 1,
  PROP_CONTEXT,
  N_PROPERTIES,
};

static GParamSpec* obj_properties[N_PROPERTIES] = { NULL };

static void texture_destruction_callback(void* user_data) {}

static struct PageTexture* page_get_texture(ExpidusRuntimeCompositorEglRenderer* self, size_t width, size_t height, gboolean make_fbo) {
  struct Page* page = &self->priv->pages[self->priv->current_page];

  for (GList* item = page->unused_textures; item != NULL; item = item->next) {
    struct PageTexture* pg_texture = item->data;
    if (pg_texture->width == width && pg_texture->height == height) {
      page->unused_textures = g_list_delete_link(page->unused_textures, item);
      return pg_texture;
    }
  }

  GLuint fbo = 0;
  if (make_fbo) {
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  }

  GLuint tex = 0;
  glGenTextures(1, &tex);

  glBindTexture(GL_TEXTURE_2D, tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA_EXT, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (make_fbo) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  glBindTexture(GL_TEXTURE_2D, 0);

  GLuint err = glGetError();
  if (err != 0) {
    g_error("Failed to create texture: %d", err);
  }

  struct PageTexture* pg_texture = g_malloc0(sizeof (struct PageTexture));
  g_assert(pg_texture != NULL);

  pg_texture->page = page;
  pg_texture->texture = tex;
  pg_texture->fbo = fbo;
  pg_texture->width = width;
  pg_texture->height = height;

  page->textures = g_list_append(page->textures, pg_texture);
  return pg_texture;
}

static void page_texture_destroy(struct PageTexture* pg_texture) {
  glDeleteTextures(1, &pg_texture->texture);
  g_free(pg_texture);
}

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

static bool expidus_runtime_compositor_egl_renderer_create_backing_store_callback(const FlutterBackingStoreConfig* config, FlutterBackingStore* backing_store_out, void* user_data) {
  ExpidusRuntimeCompositorEglRenderer* self = EXPIDUS_RUNTIME_COMPOSITOR_EGL_RENDERER(user_data);
  g_assert(self != NULL);

  struct PageTexture* pg_texture = page_get_texture(self, config->size.width, config->size.height, FALSE);

  backing_store_out->struct_size = sizeof (FlutterBackingStore);
  backing_store_out->user_data = pg_texture;
  backing_store_out->type = kFlutterBackingStoreTypeOpenGL;
  backing_store_out->did_update = FALSE;

  backing_store_out->open_gl.type = kFlutterOpenGLTargetTypeTexture;
  backing_store_out->open_gl.texture.target = GL_TEXTURE_2D;
  backing_store_out->open_gl.texture.name = pg_texture->texture;
  backing_store_out->open_gl.texture.format = 0x93A1;
  backing_store_out->open_gl.texture.user_data = pg_texture;
  backing_store_out->open_gl.texture.destruction_callback = texture_destruction_callback;
  backing_store_out->open_gl.texture.width = pg_texture->width;
  backing_store_out->open_gl.texture.height = pg_texture->height;
  return TRUE;
}

static bool expidus_runtime_compositor_egl_renderer_collect_backing_store_callback(const FlutterBackingStore* backing_store, void* user_data) {
  return TRUE;
}

static bool expidus_runtime_compositor_egl_renderer_present_layers_callback(const FlutterLayer** layers, size_t n_layers, void* user_data) {
  ExpidusRuntimeCompositorEglRenderer* self = EXPIDUS_RUNTIME_COMPOSITOR_EGL_RENDERER(user_data);
  g_assert(self != NULL);

  ExpidusRuntimeCompositorBackend* backend = expidus_runtime_compositor_renderer_get_backend(EXPIDUS_RUNTIME_COMPOSITOR_RENDERER(self));
  g_assert(backend != NULL);

  ExpidusRuntimeCompositor* compositor = expidus_runtime_compositor_backend_get_compositor(backend);
  g_assert(compositor != NULL);

  expidus_runtime_compositor_scene_clean(self->priv->scene);

  for (size_t i = 0; i < n_layers; i++) {
    const FlutterLayer* layer = layers[i];
    
    if (layer->type == kFlutterLayerContentTypeBackingStore) {
      ExpidusRuntimeCompositorSceneLayer* scene_layer = expidus_runtime_compositor_egl_scene_layer_new(self, layer);
      expidus_runtime_compositor_scene_add_layer(self->priv->scene, scene_layer);
    }

    expidus_runtime_compositor_scene_layers_init(compositor, self->priv->scene, layer);
  }

  uint8_t last_page_idx;
  if (self->priv->current_page == 0) {
    last_page_idx = 1;
  } else {
    last_page_idx = 0;
  }

  struct Page* page = &self->priv->pages[self->priv->current_page];
  g_list_free_full(page->unused_textures, (GDestroyNotify)page_texture_destroy);
  page->unused_textures = NULL;

  if (self->priv->sync != 0) {
    eglDestroySync(self->priv->display, self->priv->sync);
  }

  self->priv->sync = eglCreateSync(self->priv->display, EGL_SYNC_FENCE, NULL);

  struct Page* last_page = &self->priv->pages[last_page_idx];
  last_page->unused_textures = last_page->textures;
  last_page->textures = NULL;

  if (self->priv->current_page == 0) {
    self->priv->current_page = 1;
  } else {
    self->priv->current_page = 0;
  }
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

static void expidus_runtime_compositor_egl_renderer_wait_sync(ExpidusRuntimeCompositorRenderer* renderer) {
  ExpidusRuntimeCompositorEglRenderer* self = EXPIDUS_RUNTIME_COMPOSITOR_EGL_RENDERER(renderer);

  if (self->priv->sync != 0) {
    eglWaitSync(self->priv->display, self->priv->sync, 0);
    eglDestroySync(self->priv->display, self->priv->sync);
    self->priv->sync = 0;
  }
}

static void expidus_runtime_compositor_egl_renderer_render(ExpidusRuntimeCompositorRenderer* renderer) {
  ExpidusRuntimeCompositorEglRenderer* self = EXPIDUS_RUNTIME_COMPOSITOR_EGL_RENDERER(renderer);
  expidus_runtime_compositor_scene_render(self->priv->scene);
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

  self->priv->scene = expidus_runtime_compositor_scene_new();

  eglMakeCurrent(self->priv->display, EGL_NO_SURFACE, EGL_NO_SURFACE, self->priv->flutter_context);

  GError* error = NULL;
  self->priv->shader = expidus_runtime_compositor_egl_shader_new(&error);
  if (self->priv->shader == NULL) g_error("%s", error->message);

  const GLfloat x1 = 0.0;
  const GLfloat x2 = 1.0;
  const GLfloat y1 = 0.0;
  const GLfloat y2 = 1.0;

  const GLfloat texcoords[] = {
    x2, y2,
    x1, y2,
    x2, y1,
    x1, y1
  };

  const GLfloat quad_verts[8] = {
    1, -1, // top right
    -1, -1, // top left
    1, 1, // bottom right
    -1, 1, // bottom left
  };

  glGenBuffers(1, &self->priv->tex_coord_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, self->priv->tex_coord_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof (texcoords), texcoords, GL_STATIC_DRAW);

  glGenBuffers(1, &self->priv->quad_vert_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, self->priv->quad_vert_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof (quad_verts), quad_verts, GL_STATIC_DRAW);

  eglMakeCurrent(self->priv->display, EGL_NO_SURFACE, EGL_NO_SURFACE, NULL);
}

static void expidus_runtime_compositor_egl_renderer_dispose(GObject* object) {
  ExpidusRuntimeCompositorEglRenderer* self = EXPIDUS_RUNTIME_COMPOSITOR_EGL_RENDERER(object);

  g_clear_object(&self->priv->scene);

  if (self->priv->shader != NULL) {
    eglMakeCurrent(self->priv->display, EGL_NO_SURFACE, EGL_NO_SURFACE, self->priv->flutter_context);
    g_clear_object(&self->priv->shader);
    eglMakeCurrent(self->priv->display, EGL_NO_SURFACE, EGL_NO_SURFACE, NULL);
  }

  G_OBJECT_CLASS(expidus_runtime_compositor_egl_renderer_parent_class)->dispose(object);
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
  renderer_class->wait_sync = expidus_runtime_compositor_egl_renderer_wait_sync;
  renderer_class->render = expidus_runtime_compositor_egl_renderer_render;

  object_class->constructed = expidus_runtime_compositor_egl_renderer_constructed;
  object_class->dispose = expidus_runtime_compositor_egl_renderer_dispose;
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
  self->priv->compositor.create_backing_store_callback = expidus_runtime_compositor_egl_renderer_create_backing_store_callback;
  self->priv->compositor.collect_backing_store_callback = expidus_runtime_compositor_egl_renderer_collect_backing_store_callback;
  self->priv->compositor.present_layers_callback = expidus_runtime_compositor_egl_renderer_present_layers_callback;
}

ExpidusRuntimeCompositorRenderer* expidus_runtime_compositor_egl_renderer_new(ExpidusRuntimeCompositorBackend* backend, EGLDisplay display, EGLContext context) {
  return g_object_new(EXPIDUS_RUNTIME_COMPOSITOR_TYPE_EGL_RENDERER, "backend", backend, "display", display, "context", context, NULL);
}
