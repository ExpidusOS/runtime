#pragma once

#include <expidus/runtime/compositor/rendering/scene-layer.h>

typedef struct _ExpidusRuntimeCompositorSceneLayerPrivate {
  FlutterPoint offset;
  FlutterSize size;
} ExpidusRuntimeCompositorSceneLayerPrivate;
