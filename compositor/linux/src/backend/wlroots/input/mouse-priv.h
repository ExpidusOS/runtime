#pragma once

#include <expidus/runtime/compositor/backend/wlroots/input/mouse.h>
#include <wlr/types/wlr_cursor.h>

typedef struct _ExpidusRuntimeCompositorWlrootsMouseInputPrivate {
  struct wlr_cursor* cursor;
  struct wl_listener motion;
  struct wl_listener motion_absolute;
  struct wl_listener button;
  struct wl_listener axis;
  struct wl_listener frame;

  uint32_t btn_mask;
  uint32_t last_btn_mask;

  double scroll_delta_x;
  double scroll_delta_y;

  ExpidusRuntimeCompositorWlrootsMouseInput* self;
} ExpidusRuntimeCompositorWlrootsMouseInputPrivate;
