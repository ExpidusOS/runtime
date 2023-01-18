#pragma once

#include <expidus/runtime/compositor/backend/wlroots/backend.h>
#include <wlr/backend.h>
#include <wlr/render/allocator.h>
#include <wlr/render/pixman.h>
#include <wlr/render/wlr_renderer.h>
#include <wlr/types/wlr_compositor.h>
#include <wlr/types/wlr_data_device.h>
#include <wlr/types/wlr_output_layout.h>
#include <wlr/types/wlr_presentation_time.h>
#include <wlr/types/wlr_seat.h>
#include <wlr/types/wlr_xcursor_manager.h>
#include <wlr/types/wlr_xdg_shell.h>
#include <wayland-server-core.h>

#ifdef HAS_GLES2
#include <wlr/render/egl.h>
#include <wlr/render/gles2.h>
#include <expidus/runtime/compositor/renderer/egl/renderer.h>
#endif

#ifdef HAS_VULKAN
#include <wlr/render/vulkan.h>
#endif

typedef struct _ExpidusRuntimeCompositorWlrootsBackendPrivate {
  struct wl_display* display;
  struct wl_event_loop* event_loop;
  struct wlr_backend* backend;
  struct wlr_renderer* wl_renderer;
  struct wlr_allocator* allocator;
  struct wlr_output_layout* output_layout;
  struct wlr_xdg_shell* xdg_shell;
  struct wlr_presentation* presentation;
  struct wlr_seat* seat;
  struct wlr_xcursor_manager* xcursor_mngr;

  struct wl_listener destroy;
  struct wl_listener output_new;
  struct wl_listener input_new;
  struct wl_listener xdg_surface_new;

  GList* outputs;
  GList* inputs;

  const char* socket;
  ExpidusRuntimeCompositorRenderer* renderer;
  ExpidusRuntimeCompositorBackend* self;
} ExpidusRuntimeCompositorWlrootsBackendPrivate;
