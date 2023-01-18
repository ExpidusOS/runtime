#pragma once

#include <expidus/runtime/compositor/backend/wlroots/input/keyboard.h>

typedef struct _ExpidusRuntimeCompositorWlrootsKeyboardInputPrivate {
  struct wl_listener key;

  ExpidusRuntimeCompositorWlrootsKeyboardInput* self;
} ExpidusRuntimeCompositorWlrootsKeyboardInputPrivate;
