#include <expidus/runtime/compositor/input/keyboard.h>
#include "../backend-priv.h"
#include "../compositor-priv.h"

G_DEFINE_INTERFACE(ExpidusRuntimeCompositorKeyboardInput, expidus_runtime_compositor_keyboard_input, EXPIDUS_RUNTIME_COMPOSITOR_TYPE_INPUT);

static void expidus_runtime_compositor_keyboard_input_default_init(ExpidusRuntimeCompositorKeyboardInputInterface* iface) {}

void expidus_runtime_compositor_keyboard_input_trigger(ExpidusRuntimeCompositorKeyboardInput* self, FlutterKeyEvent event) {
  g_return_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_KEYBOARD_INPUT(self));

  ExpidusRuntimeCompositorBackend* backend = NULL;
  g_object_get(self, "backend", &backend, NULL);
  g_assert(backend != NULL);

  ExpidusRuntimeCompositor* compositor = expidus_runtime_compositor_backend_get_compositor(backend);
  g_assert(compositor != NULL);

  // RFC: maybe we need to add the callback
  FlutterEngineResult result = FlutterEngineSendKeyEvent(compositor->priv->engine, &event, NULL, NULL);
  if (result != kSuccess) g_warning("Failed to send a key event (%d)", result);
}
