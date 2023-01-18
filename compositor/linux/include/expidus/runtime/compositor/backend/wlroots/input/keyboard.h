#pragma once

#include <glib-object.h>
#include <expidus/runtime/compositor/backend/wlroots/input.h>
#include <expidus/runtime/compositor/input/keyboard.h>

G_BEGIN_DECLS

#define EXPIDUS_RUNTIME_COMPOSITOR_TYPE_WLROOTS_KEYBOARD_INPUT expidus_runtime_compositor_wlroots_keyboard_input_get_type()
G_DECLARE_FINAL_TYPE(ExpidusRuntimeCompositorWlrootsKeyboardInput, expidus_runtime_compositor_wlroots_keyboard_input, EXPIDUS_RUNTIME_COMPOSITOR, WLROOTS_KEYBOARD_INPUT, ExpidusRuntimeCompositorWlrootsInput);

struct _ExpidusRuntimeCompositorWlrootsKeyboardInput {
  ExpidusRuntimeCompositorWlrootsInput parent_instance;
  struct _ExpidusRuntimeCompositorWlrootsKeyboardInputPrivate* priv;
};

G_END_DECLS
