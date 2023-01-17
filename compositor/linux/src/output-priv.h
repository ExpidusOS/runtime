#pragma once

#include <expidus/runtime/compositor/backend.h>
#include <expidus/runtime/compositor/output.h>

typedef struct _ExpidusRuntimeCompositorOutputPrivate {
  ExpidusRuntimeCompositorBackend* backend;
} ExpidusRuntimeCompositorOutputPrivate;
