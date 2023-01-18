#pragma once

#include <expidus/runtime/compositor/renderer/egl/renderer.h>
#include "renderer-priv.h"

typedef struct _ExpidusRuntimeCompositorEglSceneLayerPrivate {
  ExpidusRuntimeCompositorEglRenderer* renderer;
  struct PageTexture* pg_texture;
} ExpidusRuntimeCompositorEglSceneLayerPrivate;

ExpidusRuntimeCompositorSceneLayer* expidus_runtime_compositor_egl_scene_layer_new(ExpidusRuntimeCompositorEglRenderer* egl_renderer, const FlutterLayer* layer, struct PageTexture* pg_texture);
