#pragma once

#include <expidus/runtime/compositor/backend/wlroots/backend.h>
#include <wlr/backend.h>
#include <wayland-server-core.h>

typedef struct _ExpidusRuntimeCompositorWlrootsBackendPrivate {
  struct wl_display* display;
  struct wl_event_loop* event_loop;
  struct wlr_backend* backend;
} ExpidusRuntimeCompositorWlrootsBackendPrivate;
