#pragma once

#include <expidus/runtime/compositor/renderer/egl/renderer.h>

typedef struct _ExpidusRuntimeCompositorEglSceneLayerPrivate {
  ExpidusRuntimeCompositorEglRenderer* renderer;
  FlutterLayer* layer;
} ExpidusRuntimeCompositorEglSceneLayerPrivate;
