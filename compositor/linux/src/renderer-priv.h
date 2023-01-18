#pragma once

#include <expidus/runtime/compositor/backend.h>
#include <expidus/runtime/compositor/renderer.h>

typedef struct _ExpidusRuntimeCompositorRendererPrivate {
  ExpidusRuntimeCompositorBackend* backend;
} ExpidusRuntimeCompositorRendererPrivate;
