#include <expidus/runtime/compositor/input/mouse.h>
#include "../backend-priv.h"
#include "../compositor-priv.h"

G_DEFINE_INTERFACE(ExpidusRuntimeCompositorMouseInput, expidus_runtime_compositor_mouse_input, EXPIDUS_RUNTIME_COMPOSITOR_TYPE_INPUT);

static void expidus_runtime_compositor_mouse_input_default_init(ExpidusRuntimeCompositorMouseInputInterface* iface) {}

void expidus_runtime_compositor_mouse_input_trigger(ExpidusRuntimeCompositorMouseInput* self, FlutterPointerEvent event) {
  g_return_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_MOUSE_INPUT(self));

  ExpidusRuntimeCompositorBackend* backend = NULL;
  g_object_get(self, "backend", &backend, NULL);
  g_assert(backend != NULL);

  ExpidusRuntimeCompositor* compositor = expidus_runtime_compositor_backend_get_compositor(backend);
  g_assert(compositor != NULL);

  event.struct_size = sizeof (FlutterPointerEvent);
  event.device_kind = kFlutterPointerDeviceKindMouse;
  event.timestamp = FlutterEngineGetCurrentTime();

  FlutterEngineResult result = FlutterEngineSendPointerEvent(compositor->priv->engine, &event, 1);
  if (result != kSuccess) g_warning("Failed to send a pointer event (%d)", result);
}
