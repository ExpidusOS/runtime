#pragma once

#include <stdatomic.h>
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

  atomic_intptr_t vsync_baton;
} ExpidusRuntimeCompositorPrivate;

void expidus_runtime_compositor_vsync(ExpidusRuntimeCompositor* self);
