#include <expidus/runtime/compositor/backend/wlroots/output.h>
#include <wlr/types/wlr_output.h>
#include <wlr/types/wlr_output_layout.h>
#include <wlr/render/wlr_renderer.h>
#include "backend-priv.h"
#include "output-priv.h"

G_DEFINE_TYPE_WITH_PRIVATE(ExpidusRuntimeCompositorWlrootsOutput, expidus_runtime_compositor_wlroots_output, EXPIDUS_RUNTIME_COMPOSITOR_TYPE_OUTPUT);

enum {
  PROP_VALUE = 1,
  N_PROPERTIES,
};

static GParamSpec* obj_properties[N_PROPERTIES] = { NULL };

static void expidus_runtime_compositor_wlroots_output_get_property(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec) {
  ExpidusRuntimeCompositorWlrootsOutput* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_OUTPUT(object);

  switch (prop_id) {
    case PROP_VALUE:
      g_value_set_pointer(value, self->priv->value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
      break;
  }
}

static void expidus_runtime_compositor_wlroots_output_set_property(GObject* object, guint prop_id, const GValue* value, GParamSpec* pspec) {
  ExpidusRuntimeCompositorWlrootsOutput* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_OUTPUT(object);

  switch (prop_id) {
    case PROP_VALUE:
      self->priv->value = g_value_get_pointer(value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
      break;
  }
}

static void expidus_runtime_compositor_wlroots_output_class_init(ExpidusRuntimeCompositorWlrootsOutputClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  object_class->get_property = expidus_runtime_compositor_wlroots_output_get_property;
  object_class->set_property = expidus_runtime_compositor_wlroots_output_set_property;

  obj_properties[PROP_VALUE] = g_param_spec_pointer("value", "Value", "An instance of a wlr_output", G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}

static void expidus_runtime_compositor_wlroots_output_init(ExpidusRuntimeCompositorWlrootsOutput* self) {
  ExpidusRuntimeCompositorWlrootsOutputPrivate* priv = expidus_runtime_compositor_wlroots_output_get_instance_private(self);
  g_assert(priv != NULL);
  self->priv = priv;
  priv->self = EXPIDUS_RUNTIME_COMPOSITOR_OUTPUT(self);
}

ExpidusRuntimeCompositorOutput* expidus_runtime_compositor_wlroots_output_new(ExpidusRuntimeCompositorWlrootsBackend* backend, struct wlr_output* value) {
  return g_object_new(EXPIDUS_RUNTIME_COMPOSITOR_TYPE_WLROOTS_OUTPUT, "backend", backend, "value", value, NULL);
}

void expidus_runtime_compositor_wlroots_output_new_cb(struct wl_listener* listener, void* data) {
  ExpidusRuntimeCompositorWlrootsBackendPrivate* backend_priv = wl_container_of(listener, backend_priv, output_new);
  struct wlr_output* output = data;

  wlr_output_init_render(output, backend_priv->allocator, backend_priv->wl_renderer);
  
  ExpidusRuntimeCompositorOutput* self = expidus_runtime_compositor_wlroots_output_new(EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_BACKEND(backend_priv->self), output);
  backend_priv->outputs = g_list_append(backend_priv->outputs, self);

  if (!wl_list_empty(&output->modes)) {
    struct wlr_output_mode* mode = wlr_output_preferred_mode(output);
    wlr_output_set_mode(output, mode);
    wlr_output_enable(output, TRUE);
    wlr_output_enable_adaptive_sync(output, TRUE);

    if (!wlr_output_commit(output)) {
      return;
    }
  }

  wlr_output_layout_add_auto(backend_priv->output_layout, output);
}
