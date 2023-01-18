#include <expidus/runtime/compositor/backend/wlroots/input.h>
#include <expidus/runtime/compositor/backend/wlroots/input/keyboard.h>
#include <expidus/runtime/compositor/backend/wlroots/input/mouse.h>
#include "backend-priv.h"
#include "input-priv.h"

G_DEFINE_TYPE_WITH_PRIVATE(ExpidusRuntimeCompositorWlrootsInput, expidus_runtime_compositor_wlroots_input, EXPIDUS_RUNTIME_COMPOSITOR_TYPE_INPUT);

enum {
  PROP_DEVICE = 1,
  N_PROPERTIES,
};

static GParamSpec* obj_properties[N_PROPERTIES] = { NULL };

static gboolean expidus_runtime_compositor_wlroots_input_compare(ExpidusRuntimeCompositorWlrootsInput* a, ExpidusRuntimeCompositorWlrootsInput* b) {
  ExpidusRuntimeCompositorWlrootsInputPrivate* a_priv = expidus_runtime_compositor_wlroots_input_get_instance_private(a);
  ExpidusRuntimeCompositorWlrootsInputPrivate* b_priv = expidus_runtime_compositor_wlroots_input_get_instance_private(b);
  return !(a_priv->device == b_priv->device);
}

static void expidus_runtime_compositor_wlroots_input_destroy(struct wl_listener* listener, void* user_data) {
  ExpidusRuntimeCompositorWlrootsInputPrivate* priv = wl_container_of(listener, priv, destroy);
  ExpidusRuntimeCompositorWlrootsBackend* backend = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_BACKEND(expidus_runtime_compositor_input_get_backend(EXPIDUS_RUNTIME_COMPOSITOR_INPUT(priv->self)));

  GList* inputs = backend->priv->inputs;
  
  GList* link = g_list_find_custom(inputs, priv->self, (GCompareFunc)expidus_runtime_compositor_wlroots_input_compare);
  g_assert(link != NULL);

  backend->priv->inputs = g_list_delete_link(inputs, link);
  g_object_unref(priv->self);
}

static const gchar* expidus_runtime_compositor_wlroots_input_get_name(ExpidusRuntimeCompositorInput* input) {
  ExpidusRuntimeCompositorWlrootsInput* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_INPUT(input);

  ExpidusRuntimeCompositorWlrootsInputPrivate* priv = expidus_runtime_compositor_wlroots_input_get_instance_private(self);
  g_assert(priv != NULL);
  return priv->device->name;
}

static void expidus_runtime_compositor_wlroots_input_constructed(GObject* object) {
  G_OBJECT_CLASS(expidus_runtime_compositor_wlroots_input_parent_class)->constructed(object);

  ExpidusRuntimeCompositorWlrootsInput* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_INPUT(object);
  ExpidusRuntimeCompositorWlrootsInputPrivate* priv = expidus_runtime_compositor_wlroots_input_get_instance_private(self);
  g_assert(priv != NULL);

  priv->self = self;
  priv->destroy.notify = expidus_runtime_compositor_wlroots_input_destroy;
  wl_signal_add(&priv->device->events.destroy, &priv->destroy);
}

static void expidus_runtime_compositor_wlroots_input_finalize(GObject* object) {
  ExpidusRuntimeCompositorWlrootsInput* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_INPUT(object);
  ExpidusRuntimeCompositorWlrootsInputPrivate* priv = expidus_runtime_compositor_wlroots_input_get_instance_private(self);
  g_assert(priv != NULL);

  wl_list_remove(&priv->destroy.link);

  G_OBJECT_CLASS(expidus_runtime_compositor_wlroots_input_parent_class)->finalize(object);
}

static void expidus_runtime_compositor_wlroots_input_get_property(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec) {
  ExpidusRuntimeCompositorWlrootsInput* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_INPUT(object);
  ExpidusRuntimeCompositorWlrootsInputPrivate* priv = expidus_runtime_compositor_wlroots_input_get_instance_private(self);
  g_assert(priv != NULL);

  switch (prop_id) {
    case PROP_DEVICE:
      g_value_set_pointer(value, priv->device);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
      break;
  }
}

static void expidus_runtime_compositor_wlroots_input_set_property(GObject* object, guint prop_id, const GValue* value, GParamSpec* pspec) {
  ExpidusRuntimeCompositorWlrootsInput* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_INPUT(object);
  ExpidusRuntimeCompositorWlrootsInputPrivate* priv = expidus_runtime_compositor_wlroots_input_get_instance_private(self);
  g_assert(priv != NULL);

  switch (prop_id) {
    case PROP_DEVICE:
      priv->device = g_value_get_pointer(value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
      break;
  }
}

static void expidus_runtime_compositor_wlroots_input_class_init(ExpidusRuntimeCompositorWlrootsInputClass* klass) {
  ExpidusRuntimeCompositorInputClass* input_class = EXPIDUS_RUNTIME_COMPOSITOR_INPUT_CLASS(klass);
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  input_class->get_name = expidus_runtime_compositor_wlroots_input_get_name;

  object_class->constructed = expidus_runtime_compositor_wlroots_input_constructed;
  object_class->finalize = expidus_runtime_compositor_wlroots_input_finalize;
  object_class->get_property = expidus_runtime_compositor_wlroots_input_get_property;
  object_class->set_property = expidus_runtime_compositor_wlroots_input_set_property;

  obj_properties[PROP_DEVICE] = g_param_spec_pointer("device", "Device", "The wlr_input_device being used", G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}

static void expidus_runtime_compositor_wlroots_input_init(ExpidusRuntimeCompositorWlrootsInput* self) {}

struct wlr_input_device* expidus_runtime_compositor_wlroots_input_get_device(ExpidusRuntimeCompositorWlrootsInput* self) {
  g_return_val_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_WLROOTS_INPUT(self), NULL);

  ExpidusRuntimeCompositorWlrootsInputPrivate* priv = expidus_runtime_compositor_wlroots_input_get_instance_private(self);
  g_assert(priv != NULL);
  return priv->device;
}

void expidus_runtime_compositor_wlroots_input_new_cb(struct wl_listener* listener, void* data) {
  ExpidusRuntimeCompositorWlrootsBackendPrivate* backend_priv = wl_container_of(listener, backend_priv, input_new);
  struct wlr_input_device* input = data;

  ExpidusRuntimeCompositorWlrootsInput* self = NULL;

  switch (input->type) {
    case WLR_INPUT_DEVICE_POINTER:
      self = g_object_new(EXPIDUS_RUNTIME_COMPOSITOR_TYPE_WLROOTS_MOUSE_INPUT, "backend", backend_priv->self, "device", input, NULL);
      wlr_seat_set_capabilities(backend_priv->seat, backend_priv->seat->capabilities | WL_SEAT_CAPABILITY_POINTER);
      break;
    case WLR_INPUT_DEVICE_KEYBOARD:
      self = g_object_new(EXPIDUS_RUNTIME_COMPOSITOR_TYPE_WLROOTS_KEYBOARD_INPUT, "backend", backend_priv->self, "device", input, NULL);
      wlr_seat_set_capabilities(backend_priv->seat, backend_priv->seat->capabilities | WL_SEAT_CAPABILITY_KEYBOARD);
      break;
    default:
      g_debug("Not handling %s (%d)", input->name, input->type);
      break;
  }

  if (self == NULL) return;
  backend_priv->inputs = g_list_append(backend_priv->inputs, self);
}
