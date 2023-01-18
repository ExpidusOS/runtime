#pragma once

#include <expidus/runtime/compositor/rendering/scene.h>

typedef struct _ExpidusRuntimeCompositorScenePrivate {
  pthread_mutex_t mutex;
  GList* layers;
} ExpidusRuntimeCompositorScenePrivate;
