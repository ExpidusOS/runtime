#pragma once

#include <glib-object.h>
#include <expidus/runtime/compositor/input.h>

G_BEGIN_DECLS

#define EXPIDUS_RUNTIME_COMPOSITOR_TYPE_MOUSE_INPUT expidus_runtime_compositor_mouse_input_get_type()
G_DECLARE_INTERFACE(ExpidusRuntimeCompositorMouseInput, expidus_runtime_compositor_mouse_input, EXPIDUS_RUNTIME_COMPOSITOR, MOUSE_INPUT, ExpidusRuntimeCompositorInput);

struct _ExpidusRuntimeCompositorMouseInputInterface {
  GTypeInterface parent_iface;
};

void expidus_runtime_compositor_mouse_input_trigger(ExpidusRuntimeCompositorMouseInput* self, FlutterPointerEvent event);

G_END_DECLS
