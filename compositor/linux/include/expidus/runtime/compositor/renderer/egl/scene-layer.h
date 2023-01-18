#pragma once

#include <glib-object.h>
#include <expidus/runtime/compositor/renderer/egl/renderer.h>
#include <expidus/runtime/compositor/rendering/scene-layer.h>

G_BEGIN_DECLS

#define EXPIDUS_RUNTIME_COMPOSITOR_TYPE_EGL_SCENE_LAYER expidus_runtime_compositor_egl_scene_layer_get_type()
G_DECLARE_FINAL_TYPE(ExpidusRuntimeCompositorEglSceneLayer, expidus_runtime_compositor_egl_scene_layer, EXPIDUS_RUNTIME_COMPOSITOR, EGL_SCENE_LAYER, ExpidusRuntimeCompositorSceneLayer);

struct _ExpidusRuntimeCompositorEglSceneLayer {
  ExpidusRuntimeCompositorSceneLayer parent_instance;
  struct _ExpidusRuntimeCompositorEglSceneLayerPrivate* priv;
};

ExpidusRuntimeCompositorSceneLayer* expidus_runtime_compositor_egl_scene_layer_new(ExpidusRuntimeCompositorEglRenderer* egl_renderer, const FlutterLayer* layer);

G_END_DECLS
