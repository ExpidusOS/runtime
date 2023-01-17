#pragma once

#include <expidus/runtime/compositor/backend.h>
#include <expidus/runtime/compositor.h>

typedef struct _ExpidusRuntimeCompositorPrivate {
  ExpidusRuntimeCompositorBackend* backend;
  gchar* binary_name;

  struct {
    gchar* backend;
  } arguments;
} ExpidusRuntimeCompositorPrivate;
