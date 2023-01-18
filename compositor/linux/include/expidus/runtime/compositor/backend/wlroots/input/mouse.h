#pragma once

#include <glib-object.h>
#include <expidus/runtime/compositor/backend/wlroots/input.h>
#include <expidus/runtime/compositor/input/mouse.h>

G_BEGIN_DECLS

#define EXPIDUS_RUNTIME_COMPOSITOR_TYPE_WLROOTS_MOUSE_INPUT expidus_runtime_compositor_wlroots_mouse_input_get_type()
G_DECLARE_FINAL_TYPE(ExpidusRuntimeCompositorWlrootsMouseInput, expidus_runtime_compositor_wlroots_mouse_input, EXPIDUS_RUNTIME_COMPOSITOR, WLROOTS_MOUSE_INPUT, ExpidusRuntimeCompositorWlrootsInput);

struct _ExpidusRuntimeCompositorWlrootsMouseInput {
  ExpidusRuntimeCompositorWlrootsInput parent_instance;
  struct _ExpidusRuntimeCompositorWlrootsMouseInputPrivate* priv;
};

G_END_DECLS
