#include <expidus/runtime/compositor/backend/wlroots/input/keyboard.h>
#include <wlr/types/wlr_keyboard.h>
#include "keyboard-priv.h"
#include "../backend-priv.h"

static void expidus_runtime_compositor_keyboard_input_interface_init(ExpidusRuntimeCompositorKeyboardInputInterface* iface);

G_DEFINE_TYPE_WITH_CODE(ExpidusRuntimeCompositorWlrootsKeyboardInput, expidus_runtime_compositor_wlroots_keyboard_input, EXPIDUS_RUNTIME_COMPOSITOR_TYPE_WLROOTS_INPUT,
    G_ADD_PRIVATE(ExpidusRuntimeCompositorWlrootsKeyboardInput)
    G_IMPLEMENT_INTERFACE(EXPIDUS_RUNTIME_COMPOSITOR_TYPE_KEYBOARD_INPUT, expidus_runtime_compositor_keyboard_input_interface_init));

static void expidus_runtime_compositor_wlroots_keyboard_input_key(struct wl_listener* listener, void* data) {
  ExpidusRuntimeCompositorWlrootsKeyboardInputPrivate* priv = wl_container_of(listener, priv, key);
  struct wlr_event_keyboard_key* key = data;

  FlutterKeyEvent event = {};
  event.struct_size = sizeof (FlutterKeyEvent);
  event.timestamp = FlutterEngineGetCurrentTime();

  switch (key->state) {
    case WL_KEYBOARD_KEY_STATE_PRESSED:
      event.type = kFlutterKeyEventTypeDown;
      break;
    case WL_KEYBOARD_KEY_STATE_RELEASED:
      event.type = kFlutterKeyEventTypeUp;
      break;
  }
}

static void expidus_runtime_compositor_wlroots_keyboard_input_constructed(GObject* object) {
  G_OBJECT_CLASS(expidus_runtime_compositor_wlroots_keyboard_input_parent_class)->constructed(object);

  ExpidusRuntimeCompositorWlrootsKeyboardInput* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_KEYBOARD_INPUT(object);
  g_assert(self != NULL);

  ExpidusRuntimeCompositorWlrootsBackend* backend = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_BACKEND(expidus_runtime_compositor_input_get_backend(EXPIDUS_RUNTIME_COMPOSITOR_INPUT(self)));
  g_assert(backend != NULL);

  struct wlr_input_device* device = expidus_runtime_compositor_wlroots_input_get_device(EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_INPUT(self));
  g_assert(device != NULL);
  g_assert(device->type == WLR_INPUT_DEVICE_KEYBOARD);

  struct wlr_keyboard* keyboard = device->keyboard;

  self->priv->key.notify = expidus_runtime_compositor_wlroots_keyboard_input_key;
  wl_signal_add(&keyboard->events.key, &self->priv->key);

  wlr_seat_set_keyboard(backend->priv->seat, device);
}

static void expidus_runtime_compositor_wlroots_keyboard_input_dispose(GObject* object) {
  ExpidusRuntimeCompositorWlrootsKeyboardInput* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_KEYBOARD_INPUT(object);
  g_assert(self != NULL);

  ExpidusRuntimeCompositorWlrootsBackend* backend = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_BACKEND(expidus_runtime_compositor_input_get_backend(EXPIDUS_RUNTIME_COMPOSITOR_INPUT(self)));
  if (backend != NULL) {
    wlr_seat_set_keyboard(backend->priv->seat, NULL);
    wl_list_remove(&self->priv->key.link);
  }

  G_OBJECT_CLASS(expidus_runtime_compositor_wlroots_keyboard_input_parent_class)->dispose(object);
}

static void expidus_runtime_compositor_keyboard_input_interface_init(ExpidusRuntimeCompositorKeyboardInputInterface* iface) {}

static void expidus_runtime_compositor_wlroots_keyboard_input_class_init(ExpidusRuntimeCompositorWlrootsKeyboardInputClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  object_class->constructed = expidus_runtime_compositor_wlroots_keyboard_input_constructed;
  object_class->dispose = expidus_runtime_compositor_wlroots_keyboard_input_dispose;
}

static void expidus_runtime_compositor_wlroots_keyboard_input_init(ExpidusRuntimeCompositorWlrootsKeyboardInput* self) {
  ExpidusRuntimeCompositorWlrootsKeyboardInputPrivate* priv = expidus_runtime_compositor_wlroots_keyboard_input_get_instance_private(self);
  g_assert(priv != NULL);
  self->priv = priv;
  priv->self = self;
}
