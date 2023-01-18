#pragma once

#include <expidus/runtime/compositor/backend/wlroots/input.h>

typedef struct _ExpidusRuntimeCompositorWlrootsInputPrivate {
  struct wlr_input_device* device;
  struct wl_listener destroy;

  ExpidusRuntimeCompositorWlrootsInput* self;
} ExpidusRuntimeCompositorWlrootsInputPrivate;

void expidus_runtime_compositor_wlroots_input_new_cb(struct wl_listener* listener, void* data);
