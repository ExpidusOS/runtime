#pragma once

#include <glib-object.h>
#include <wayland-server-core.h>
#include <expidus/runtime/compositor/input.h>
#include <wlr/types/wlr_input_device.h>

G_BEGIN_DECLS

#define EXPIDUS_RUNTIME_COMPOSITOR_TYPE_WLROOTS_INPUT expidus_runtime_compositor_wlroots_input_get_type()
G_DECLARE_DERIVABLE_TYPE(ExpidusRuntimeCompositorWlrootsInput, expidus_runtime_compositor_wlroots_input, EXPIDUS_RUNTIME_COMPOSITOR, WLROOTS_INPUT, ExpidusRuntimeCompositorInput);

struct _ExpidusRuntimeCompositorWlrootsInputClass {
  ExpidusRuntimeCompositorInputClass parent_class;
};

struct wlr_input_device* expidus_runtime_compositor_wlroots_input_get_device(ExpidusRuntimeCompositorWlrootsInput* self);

G_END_DECLS
