#include <expidus/runtime/compositor/renderer/egl/scene-layer.h>
#include "scene-layer-priv.h"
#include "shader-priv.h"

G_DEFINE_TYPE_WITH_PRIVATE(ExpidusRuntimeCompositorEglSceneLayer, expidus_runtime_compositor_egl_scene_layer, EXPIDUS_RUNTIME_COMPOSITOR_TYPE_SCENE_LAYER);

enum {
  PROP_RENDERER = 1,
  PROP_PAGE_TEXTURE,
  N_PROPERTIES,
};

static GParamSpec* obj_properties[N_PROPERTIES] = { NULL };

static void expidus_runtime_compositor_egl_scene_layer_render(ExpidusRuntimeCompositorSceneLayer* scene_layer) {
  ExpidusRuntimeCompositorEglSceneLayer* self = EXPIDUS_RUNTIME_COMPOSITOR_EGL_SCENE_LAYER(scene_layer);

  glUseProgram(self->priv->renderer->priv->shader->priv->prog);

  glEnableVertexAttribArray(self->priv->renderer->priv->shader->priv->pos_attrib);
  glBindBuffer(GL_ARRAY_BUFFER, self->priv->renderer->priv->quad_vert_buffer);
  glVertexAttribPointer(self->priv->renderer->priv->shader->priv->pos_attrib, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(self->priv->renderer->priv->shader->priv->tex_attrib);
  glBindBuffer(GL_ARRAY_BUFFER, self->priv->renderer->priv->tex_coord_buffer);
  glVertexAttribPointer(self->priv->renderer->priv->shader->priv->tex_attrib, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glActiveTexture(GL_TEXTURE0);

  if (self->priv->pg_texture != NULL) {
    glBindTexture(GL_TEXTURE_2D, self->priv->pg_texture->texture);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }

  glDisableVertexAttribArray(self->priv->renderer->priv->shader->priv->pos_attrib);
  glDisableVertexAttribArray(self->priv->renderer->priv->shader->priv->tex_attrib);

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glUseProgram(0);
}

static void expidus_runtime_compositor_egl_scene_layer_dispose(GObject* object) {
  ExpidusRuntimeCompositorEglSceneLayer* self = EXPIDUS_RUNTIME_COMPOSITOR_EGL_SCENE_LAYER(object);

  g_clear_object(&self->priv->renderer);

  G_OBJECT_CLASS(expidus_runtime_compositor_egl_scene_layer_parent_class)->dispose(object);
}

static void expidus_runtime_compositor_egl_scene_layer_get_property(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec) {
  ExpidusRuntimeCompositorEglSceneLayer* self = EXPIDUS_RUNTIME_COMPOSITOR_EGL_SCENE_LAYER(object);

  switch (prop_id) {
    case PROP_RENDERER:
      g_value_set_object(value, self->priv->renderer);
      break;
    case PROP_PAGE_TEXTURE:
      g_value_set_pointer(value, self->priv->pg_texture);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
      break;
  }
}

static void expidus_runtime_compositor_egl_scene_layer_set_property(GObject* object, guint prop_id, const GValue* value, GParamSpec* pspec) {
  ExpidusRuntimeCompositorEglSceneLayer* self = EXPIDUS_RUNTIME_COMPOSITOR_EGL_SCENE_LAYER(object);

  switch (prop_id) {
    case PROP_RENDERER:
      self->priv->renderer = EXPIDUS_RUNTIME_COMPOSITOR_EGL_RENDERER(g_value_dup_object(value));
      break;
    case PROP_PAGE_TEXTURE:
      self->priv->pg_texture = g_value_get_pointer(value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
      break;
  }
}

static void expidus_runtime_compositor_egl_scene_layer_class_init(ExpidusRuntimeCompositorEglSceneLayerClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);
  ExpidusRuntimeCompositorSceneLayerClass* scene_layer_class = EXPIDUS_RUNTIME_COMPOSITOR_SCENE_LAYER_CLASS(klass);

  object_class->dispose = expidus_runtime_compositor_egl_scene_layer_dispose;
  object_class->get_property = expidus_runtime_compositor_egl_scene_layer_get_property;
  object_class->set_property = expidus_runtime_compositor_egl_scene_layer_set_property;

  scene_layer_class->render = expidus_runtime_compositor_egl_scene_layer_render;

  obj_properties[PROP_RENDERER] = g_param_spec_object("renderer", "Renderer", "The EGL renderer", EXPIDUS_RUNTIME_COMPOSITOR_TYPE_EGL_RENDERER, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  obj_properties[PROP_PAGE_TEXTURE] = g_param_spec_pointer("page-texture", "Page Texture", "An EGL renderer page texture", G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}

static void expidus_runtime_compositor_egl_scene_layer_init(ExpidusRuntimeCompositorEglSceneLayer* self) {
  ExpidusRuntimeCompositorEglSceneLayerPrivate* priv = expidus_runtime_compositor_egl_scene_layer_get_instance_private(self);
  self->priv = priv;
}

ExpidusRuntimeCompositorSceneLayer* expidus_runtime_compositor_egl_scene_layer_new(ExpidusRuntimeCompositorEglRenderer* egl_renderer, const FlutterLayer* layer, struct PageTexture* pg_texture) {
  return g_object_new(EXPIDUS_RUNTIME_COMPOSITOR_TYPE_EGL_SCENE_LAYER, "renderer", egl_renderer, "layer", layer, "page-texture", pg_texture, NULL);
}
