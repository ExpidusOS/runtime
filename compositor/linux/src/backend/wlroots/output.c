#include <expidus/runtime/compositor/backend/wlroots/output.h>
#include <expidus/runtime/compositor.h>
#include <wlr/types/wlr_output.h>
#include <wlr/types/wlr_output_layout.h>
#include <wlr/render/wlr_renderer.h>
#include "../../compositor-priv.h"
#include "backend-priv.h"
#include "output-priv.h"

G_DEFINE_TYPE_WITH_PRIVATE(ExpidusRuntimeCompositorWlrootsOutput, expidus_runtime_compositor_wlroots_output, EXPIDUS_RUNTIME_COMPOSITOR_TYPE_OUTPUT);

enum {
  PROP_VALUE = 1,
  N_PROPERTIES,
};

static GParamSpec* obj_properties[N_PROPERTIES] = { NULL };

static gboolean expidus_runtime_compositor_wlroots_output_compare(ExpidusRuntimeCompositorWlrootsOutput* a, ExpidusRuntimeCompositorWlrootsOutput* b) {
  return !(a->priv->value == b->priv->value);
}

static void expidus_runtime_compositor_wlroots_output_destroy(struct wl_listener* listener, void* user_data) {
  ExpidusRuntimeCompositorWlrootsOutputPrivate* priv = wl_container_of(listener, priv, destroy);
  ExpidusRuntimeCompositorWlrootsBackend* backend = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_BACKEND(expidus_runtime_compositor_output_get_backend(EXPIDUS_RUNTIME_COMPOSITOR_OUTPUT(priv->self)));
  ExpidusRuntimeCompositor* compositor = expidus_runtime_compositor_backend_get_compositor(EXPIDUS_RUNTIME_COMPOSITOR_BACKEND(backend));

  GList* outputs = backend->priv->outputs;
  
  GList* link = g_list_find_custom(outputs, priv->self, (GCompareFunc)expidus_runtime_compositor_wlroots_output_compare);
  g_assert(link != NULL);

  backend->priv->outputs = g_list_delete_link(outputs, link);
  
  expidus_runtime_compositor_backend_display_update(compositor, kFlutterEngineDisplaysUpdateTypeStartup);
  g_object_unref(priv->self);
}

static FlutterEngineDisplay* expidus_runtime_compositor_wlroots_output_get_engine(ExpidusRuntimeCompositorOutput* output) {
  ExpidusRuntimeCompositorWlrootsOutput* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_OUTPUT(output);
  g_assert(self != NULL);

  if (self->priv->value->current_mode != NULL) {
    self->priv->engine.refresh_rate = self->priv->value->current_mode->refresh * 1.0;
  } else {
    self->priv->engine.refresh_rate = self->priv->value->refresh * 1.0;
  }

  self->priv->engine.display_id = g_str_hash(self->priv->value->name);
  return &self->priv->engine;
}

static FlutterWindowMetricsEvent expidus_runtime_compositor_wlroots_output_get_window_metrics(ExpidusRuntimeCompositorOutput* output) {
  ExpidusRuntimeCompositorWlrootsOutput* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_OUTPUT(output);
  g_assert(self != NULL);

  ExpidusRuntimeCompositorWlrootsBackend* backend = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_BACKEND(expidus_runtime_compositor_output_get_backend(output));
  g_assert(backend != NULL);

  struct wlr_output_layout_output* layout = wlr_output_layout_get(backend->priv->output_layout, self->priv->value);
  g_assert(layout != NULL);

  FlutterWindowMetricsEvent event = {};
  event.struct_size = sizeof (FlutterWindowMetricsEvent);
  event.width = self->priv->value->width;
  event.height = self->priv->value->height;
  event.pixel_ratio = self->priv->value->scale;
  event.top = layout->y;
  event.left = layout->x;
  return event;
}

static void expidus_runtime_compositor_wlroots_output_frame(struct wl_listener* listener, void* user_data) {
  ExpidusRuntimeCompositorWlrootsOutputPrivate* priv = wl_container_of(listener, priv, frame);
  ExpidusRuntimeCompositorWlrootsBackend* backend = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_BACKEND(expidus_runtime_compositor_output_get_backend(priv->self));
  g_assert(backend != NULL);
  
  ExpidusRuntimeCompositor* compositor = expidus_runtime_compositor_backend_get_compositor(EXPIDUS_RUNTIME_COMPOSITOR_BACKEND(backend));
  g_assert(compositor != NULL);

  expidus_runtime_compositor_vsync(compositor);

  wlr_output_attach_render(priv->value, NULL);
  wlr_renderer_begin(backend->priv->wl_renderer, priv->value->width, priv->value->height);

  expidus_runtime_compositor_renderer_wait_sync(backend->priv->renderer);
  expidus_runtime_compositor_renderer_render(backend->priv->renderer);
  wlr_output_render_software_cursors(priv->value, NULL);

  wlr_renderer_end(backend->priv->wl_renderer);
  wlr_output_commit(priv->value);
}

static void expidus_runtime_compositor_wlroots_output_mode(struct wl_listener* listener, void* user_data) {
}

static void expidus_runtime_compositor_wlroots_output_present(struct wl_listener* listener, void* user_data) {
}

static void expidus_runtime_compositor_wlroots_output_constructed(GObject* object) {
  ExpidusRuntimeCompositorWlrootsOutput* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_OUTPUT(object);

  self->priv->destroy.notify = expidus_runtime_compositor_wlroots_output_destroy;
  wl_signal_add(&self->priv->value->events.destroy, &self->priv->destroy);

  self->priv->frame.notify = expidus_runtime_compositor_wlroots_output_frame;
  wl_signal_add(&self->priv->value->events.frame, &self->priv->frame);

  self->priv->mode.notify = expidus_runtime_compositor_wlroots_output_mode;
  wl_signal_add(&self->priv->value->events.mode, &self->priv->mode);

  self->priv->present.notify = expidus_runtime_compositor_wlroots_output_present;
  wl_signal_add(&self->priv->value->events.present, &self->priv->present);
}

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
  ExpidusRuntimeCompositorOutputClass* output_class = EXPIDUS_RUNTIME_COMPOSITOR_OUTPUT_CLASS(klass);

  object_class->constructed = expidus_runtime_compositor_wlroots_output_constructed;
  object_class->get_property = expidus_runtime_compositor_wlroots_output_get_property;
  object_class->set_property = expidus_runtime_compositor_wlroots_output_set_property;

  output_class->get_engine = expidus_runtime_compositor_wlroots_output_get_engine;
  output_class->get_window_metrics = expidus_runtime_compositor_wlroots_output_get_window_metrics;

  obj_properties[PROP_VALUE] = g_param_spec_pointer("value", "Value", "An instance of a wlr_output", G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}

static void expidus_runtime_compositor_wlroots_output_init(ExpidusRuntimeCompositorWlrootsOutput* self) {
  ExpidusRuntimeCompositorWlrootsOutputPrivate* priv = expidus_runtime_compositor_wlroots_output_get_instance_private(self);
  g_assert(priv != NULL);

  self->priv = priv;
  priv->self = EXPIDUS_RUNTIME_COMPOSITOR_OUTPUT(self);

  self->priv->engine.struct_size = sizeof (FlutterEngineDisplay);
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
