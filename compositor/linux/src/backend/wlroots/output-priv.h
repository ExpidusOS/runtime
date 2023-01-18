#pragma once

#include <expidus/runtime/compositor/backend/wlroots/output.h>

typedef struct _ExpidusRuntimeCompositorWlrootsOutputPrivate {
  struct wlr_output* value;
  FlutterEngineDisplay engine;

  struct wl_listener frame;
  struct wl_listener mode;
  struct wl_listener present;

  ExpidusRuntimeCompositorOutput* self;
} ExpidusRuntimeCompositorWlrootsOutputPrivate;

void expidus_runtime_compositor_wlroots_output_new_cb(struct wl_listener* listener, void* data);
