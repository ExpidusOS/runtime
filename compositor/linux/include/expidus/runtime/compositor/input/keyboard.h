#pragma once

#include <glib-object.h>
#include <expidus/runtime/compositor/input.h>

G_BEGIN_DECLS

#define EXPIDUS_RUNTIME_COMPOSITOR_TYPE_KEYBOARD_INPUT expidus_runtime_compositor_keyboard_input_get_type()
G_DECLARE_INTERFACE(ExpidusRuntimeCompositorKeyboardInput, expidus_runtime_compositor_keyboard_input, EXPIDUS_RUNTIME_COMPOSITOR, KEYBOARD_INPUT, ExpidusRuntimeCompositorInput);

struct _ExpidusRuntimeCompositorKeyboardInputInterface {
  GTypeInterface parent_iface;
};

void expidus_runtime_compositor_keyboard_input_trigger(ExpidusRuntimeCompositorKeyboardInput* self, FlutterKeyEvent event);

G_END_DECLS
