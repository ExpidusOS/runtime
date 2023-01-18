#pragma once

#include <expidus/runtime/compositor/rendering/scene.h>

typedef struct _ExpidusRuntimeCompositorScenePrivate {
  GList* layers;
} ExpidusRuntimeCompositorScenePrivate;
