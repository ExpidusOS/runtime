#include <expidus/runtime/compositor/backend/wlroots/input/mouse.h>
#include <linux/input-event-codes.h>
#include <wlr/types/wlr_cursor.h>
#include "mouse-priv.h"
#include "../backend-priv.h"

static void expidus_runtime_compositor_mouse_input_interface_init(ExpidusRuntimeCompositorMouseInputInterface* iface);

G_DEFINE_TYPE_WITH_CODE(ExpidusRuntimeCompositorWlrootsMouseInput, expidus_runtime_compositor_wlroots_mouse_input, EXPIDUS_RUNTIME_COMPOSITOR_TYPE_WLROOTS_INPUT,
    G_ADD_PRIVATE(ExpidusRuntimeCompositorWlrootsMouseInput)
    G_IMPLEMENT_INTERFACE(EXPIDUS_RUNTIME_COMPOSITOR_TYPE_MOUSE_INPUT, expidus_runtime_compositor_mouse_input_interface_init));

static uint32_t uapi_mouse_button_to_flutter(uint32_t uapi_button) {
  switch (uapi_button) {
    case BTN_LEFT: return 1;
    case BTN_RIGHT: return 2;
    case BTN_MIDDLE: return 3;
    case BTN_BACK: return 4;
    case BTN_FORWARD: return 5;
    case BTN_SIDE: return 6;
    case BTN_EXTRA: return 7;
    case BTN_0: return 8;
    case BTN_1: return 9;
    case BTN_2: return 10;
    case BTN_3: return 11;
    case BTN_4: return 12;
    case BTN_5: return 13;
    case BTN_6: return 14;
    case BTN_7: return 15;
    case BTN_8: return 16;
    case BTN_9: return 17;
    default: return 0;
  }
}

static uint32_t flutter_mouse_button_to_uapi(uint32_t flutter_button) {
  switch (flutter_button) {
    case 1: return BTN_LEFT;
    case 2: return BTN_RIGHT;
    case 3: return BTN_MIDDLE;
    case 4: return BTN_BACK;
    case 5: return BTN_FORWARD;
    case 6: return BTN_SIDE;
    case 7: return BTN_EXTRA;
    case 8: return BTN_0;
    case 9: return BTN_1;
    case 10: return BTN_2;
    case 11: return BTN_3;
    case 12: return BTN_4;
    case 13: return BTN_5;
    case 14: return BTN_6;
    case 15: return BTN_7;
    case 16: return BTN_8;
    case 17: return BTN_9;
    default: return 0;
  }
}

static void expidus_runtime_compositor_wlroots_mouse_input_process_motion(ExpidusRuntimeCompositorWlrootsMouseInput* self) {
  ExpidusRuntimeCompositorWlrootsBackend* backend = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_BACKEND(expidus_runtime_compositor_input_get_backend(EXPIDUS_RUNTIME_COMPOSITOR_INPUT(self)));
  g_assert(backend != NULL);

  wlr_xcursor_manager_set_cursor_image(backend->priv->xcursor_mngr, "left_ptr", self->priv->cursor);
}

static void expidus_runtime_compositor_wlroots_mouse_input_motion(struct wl_listener* listener, void* data) {
  ExpidusRuntimeCompositorWlrootsMouseInputPrivate* priv = wl_container_of(listener, priv, motion);
  struct wlr_event_pointer_motion* event = data;

  wlr_cursor_move(priv->cursor, event->device, event->delta_x, event->delta_y);
  expidus_runtime_compositor_wlroots_mouse_input_process_motion(priv->self);
}

static void expidus_runtime_compositor_wlroots_mouse_input_motion_absolute(struct wl_listener* listener, void* data) {
  ExpidusRuntimeCompositorWlrootsMouseInputPrivate* priv = wl_container_of(listener, priv, motion_absolute);
  struct wlr_event_pointer_motion_absolute* event = data;

  wlr_cursor_warp_absolute(priv->cursor, event->device, event->x, event->y);
  expidus_runtime_compositor_wlroots_mouse_input_process_motion(priv->self);
}

static void expidus_runtime_compositor_wlroots_mouse_input_button(struct wl_listener* listener, void* data) {
  ExpidusRuntimeCompositorWlrootsMouseInputPrivate* priv = wl_container_of(listener, priv, axis);
  struct wlr_event_pointer_button* event = data;

  uint32_t fl_button = uapi_mouse_button_to_flutter(event->button);
  if (fl_button == 0) return;

  uint32_t mask = 1 << (fl_button - 1);
  switch (event->state) {
    case WLR_BUTTON_PRESSED:
      priv->btn_mask |= mask;
      break;
    case WLR_BUTTON_RELEASED:
      priv->btn_mask &= ~mask;
      break;
  }
}

static void expidus_runtime_compositor_wlroots_mouse_input_axis(struct wl_listener* listener, void* data) {
  ExpidusRuntimeCompositorWlrootsMouseInputPrivate* priv = wl_container_of(listener, priv, axis);
  struct wlr_event_pointer_axis* event = data;

  ExpidusRuntimeCompositorWlrootsBackend* backend = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_BACKEND(expidus_runtime_compositor_input_get_backend(EXPIDUS_RUNTIME_COMPOSITOR_INPUT(priv->self)));
  g_assert(backend != NULL);

  wlr_xcursor_manager_set_cursor_image(backend->priv->xcursor_mngr, "left_ptr", priv->self->priv->cursor);

  switch (event->orientation) {
    case WLR_AXIS_ORIENTATION_HORIZONTAL:
      priv->scroll_delta_x += event->delta;
      break;
    case WLR_AXIS_ORIENTATION_VERTICAL:
      priv->scroll_delta_y += event->delta;
      break;
  }
}

static void expidus_runtime_compositor_wlroots_mouse_input_frame(struct wl_listener* listener, void* data) {
  ExpidusRuntimeCompositorWlrootsMouseInputPrivate* priv = wl_container_of(listener, priv, frame);

  uint32_t last_mask = priv->last_btn_mask;
  uint32_t curr_mask = priv->btn_mask;

  FlutterPointerEvent pointer_event = {};

  if (last_mask == 0 && curr_mask != 0) pointer_event.phase = kDown;
  else if (last_mask != 0 && curr_mask == 0) pointer_event.phase = kUp;
  else if (curr_mask == 0) pointer_event.phase = kHover;
  else pointer_event.phase = kMove;

  pointer_event.x = priv->self->priv->cursor->x;
  pointer_event.y = priv->self->priv->cursor->y;
  pointer_event.device = 0; // TODO: determine our index
  pointer_event.signal_kind = kFlutterPointerSignalKindNone;
  pointer_event.buttons = curr_mask;
  pointer_event.scroll_delta_x = priv->scroll_delta_x;
  pointer_event.scroll_delta_y = priv->scroll_delta_y;
  expidus_runtime_compositor_mouse_input_trigger(EXPIDUS_RUNTIME_COMPOSITOR_MOUSE_INPUT(priv->self), pointer_event);

  priv->last_btn_mask = curr_mask;
  priv->scroll_delta_x = 0;
  priv->scroll_delta_y = 0;
}

static void expidus_runtime_compositor_wlroots_mouse_input_constructed(GObject* object) {
  G_OBJECT_CLASS(expidus_runtime_compositor_wlroots_mouse_input_parent_class)->constructed(object);

  ExpidusRuntimeCompositorWlrootsMouseInput* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_MOUSE_INPUT(object);
  g_assert(self != NULL);

  ExpidusRuntimeCompositorWlrootsBackend* backend = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_BACKEND(expidus_runtime_compositor_input_get_backend(EXPIDUS_RUNTIME_COMPOSITOR_INPUT(self)));
  g_assert(backend != NULL);

  struct wlr_input_device* device = expidus_runtime_compositor_wlroots_input_get_device(EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_INPUT(self));
  g_assert(device != NULL);
  g_assert(device->type == WLR_INPUT_DEVICE_POINTER);

  self->priv->cursor = wlr_cursor_create();
  wlr_cursor_attach_output_layout(self->priv->cursor, backend->priv->output_layout);
  wlr_cursor_attach_input_device(self->priv->cursor, device);

  self->priv->motion.notify = expidus_runtime_compositor_wlroots_mouse_input_motion;
  wl_signal_add(&self->priv->cursor->events.motion, &self->priv->motion);

  self->priv->motion_absolute.notify = expidus_runtime_compositor_wlroots_mouse_input_motion_absolute;
  wl_signal_add(&self->priv->cursor->events.motion_absolute, &self->priv->motion_absolute);

  self->priv->button.notify = expidus_runtime_compositor_wlroots_mouse_input_button;
  wl_signal_add(&self->priv->cursor->events.button, &self->priv->button);

  self->priv->axis.notify = expidus_runtime_compositor_wlroots_mouse_input_axis;
  wl_signal_add(&self->priv->cursor->events.axis, &self->priv->axis);

  self->priv->frame.notify = expidus_runtime_compositor_wlroots_mouse_input_frame;
  wl_signal_add(&self->priv->cursor->events.frame, &self->priv->frame);
}

static void expidus_runtime_compositor_wlroots_mouse_input_dispose(GObject* object) {
  ExpidusRuntimeCompositorWlrootsMouseInput* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_MOUSE_INPUT(object);
  g_assert(self != NULL);

  g_clear_pointer(&self->priv->cursor, wlr_cursor_destroy);

  G_OBJECT_CLASS(expidus_runtime_compositor_wlroots_mouse_input_parent_class)->dispose(object);
}

static void expidus_runtime_compositor_mouse_input_interface_init(ExpidusRuntimeCompositorMouseInputInterface* iface) {}

static void expidus_runtime_compositor_wlroots_mouse_input_class_init(ExpidusRuntimeCompositorWlrootsMouseInputClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  object_class->constructed = expidus_runtime_compositor_wlroots_mouse_input_constructed;
  object_class->dispose= expidus_runtime_compositor_wlroots_mouse_input_dispose;
}

static void expidus_runtime_compositor_wlroots_mouse_input_init(ExpidusRuntimeCompositorWlrootsMouseInput* self) {
  ExpidusRuntimeCompositorWlrootsMouseInputPrivate* priv = expidus_runtime_compositor_wlroots_mouse_input_get_instance_private(self);
  g_assert(priv != NULL);
  self->priv = priv;
  priv->self = self;
}
