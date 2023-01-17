#pragma once

#include <expidus/runtime/compositor/backend.h>
#include <expidus/runtime/compositor.h>

typedef struct _ExpidusRuntimeCompositorPrivate {
  FlutterEngineProcTable flutter_procs;
  FlutterProjectArgs project_args;
  FlutterEngine engine;
  ExpidusRuntimeCompositorBackend* backend;
  gchar* binary_name;
  gchar* argv0;

  struct {
    gchar* backend;
  } arguments;
} ExpidusRuntimeCompositorPrivate;
