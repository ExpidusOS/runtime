#include <expidus/runtime/compositor/backend/wlroots/backend.h>
#include <gio/gio.h>
#include <wlr/util/log.h>
#include "backend-priv.h"
#include "error-priv.h"
#include "input-priv.h"
#include "output-priv.h"

static void g_initable_interface_init(GInitableIface* interface);

G_DEFINE_TYPE_WITH_CODE(ExpidusRuntimeCompositorWlrootsBackend, expidus_runtime_compositor_wlroots_backend, EXPIDUS_RUNTIME_COMPOSITOR_TYPE_BACKEND,
    G_ADD_PRIVATE(ExpidusRuntimeCompositorWlrootsBackend)
    G_IMPLEMENT_INTERFACE(G_TYPE_INITABLE, g_initable_interface_init));

static void expidus_runtime_compositor_wlroots_destroy_cb(struct wl_listener* listener, void* data) {
  ExpidusRuntimeCompositorWlrootsBackendPrivate* backend_priv = wl_container_of(listener, backend_priv, destroy);
//  ExpidusRuntimeCompositor* compositor = expidus_runtime_compositor_backend_get_compositor(backend_priv->self);
}

static void expidus_runtime_compositor_wlroots_backend_xdg_surface_new(struct wl_listener* listener, void* data) {
  // TODO: move to surface.c
}

static gboolean expidus_runtime_compositor_wlroots_backend_renderered_init(ExpidusRuntimeCompositorWlrootsBackend* self, GCancellable* cancellable, GError** error) {
  wlr_renderer_init_wl_display(self->priv->wl_renderer, self->priv->display);

  self->priv->seat = wlr_seat_create(self->priv->display, "seat0");

  self->priv->xcursor_mngr = wlr_xcursor_manager_create(NULL, 24);
  wlr_xcursor_manager_load(self->priv->xcursor_mngr, 1);

  self->priv->allocator = wlr_allocator_autocreate(self->priv->backend, self->priv->wl_renderer);
  wlr_compositor_create(self->priv->display, self->priv->wl_renderer);
  wlr_data_device_manager_create(self->priv->display);

  self->priv->destroy.notify = expidus_runtime_compositor_wlroots_destroy_cb;
  wl_signal_add(&self->priv->backend->events.destroy, &self->priv->destroy);

  self->priv->output_layout = wlr_output_layout_create();
  self->priv->output_new.notify = expidus_runtime_compositor_wlroots_output_new_cb;
  wl_signal_add(&self->priv->backend->events.new_output, &self->priv->output_new);

  self->priv->input_new.notify = expidus_runtime_compositor_wlroots_input_new_cb;
  wl_signal_add(&self->priv->backend->events.new_input, &self->priv->input_new);

  self->priv->xdg_shell = wlr_xdg_shell_create(self->priv->display);
  self->priv->xdg_surface_new.notify = expidus_runtime_compositor_wlroots_backend_xdg_surface_new;
  wl_signal_add(&self->priv->xdg_shell->events.new_surface, &self->priv->xdg_surface_new);
 
  if ((self->priv->socket = wl_display_add_socket_auto(self->priv->display)) == NULL) {
    expidus_runtime_compositor_wlroots_error_set_wayland(error, "failed to create a socket");
    return FALSE;
  }
  
  g_debug("Wayland socket is \"%s\"", self->priv->socket);

  self->priv->presentation = wlr_presentation_create(self->priv->display, self->priv->backend);

  if (!wlr_backend_start(self->priv->backend)) {
    expidus_runtime_compositor_wlroots_error_set_wayland(error, "failed to start the backend");
    return FALSE;
  }
  return TRUE;
}

static gboolean expidus_runtime_compositor_wlroots_backend_initable_init(GInitable* initable, GCancellable* cancellable, GError** error) {
  ExpidusRuntimeCompositorWlrootsBackend* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_BACKEND(initable);
  g_assert(self != NULL);

  int drm_fd = -1;
  if (drm_fd < 0) {
    if ((drm_fd = wlr_backend_get_drm_fd(self->priv->backend)) < 0) {
      expidus_runtime_compositor_wlroots_error_set_drm(error);
      return FALSE;
    }

#ifdef HAS_VULKAN
    if (self->priv->wl_renderer == NULL) {
      if ((self->priv->wl_renderer = wlr_vk_renderer_create_with_drm_fd(drm_fd)) != NULL) {
        return expidus_runtime_compositor_wlroots_backend_renderered_init(self, cancellable, error);
      }
    }
#endif

#ifdef HAS_GLES2
    if (self->priv->wl_renderer == NULL) {
      if ((self->priv->wl_renderer = wlr_gles2_renderer_create_with_drm_fd(drm_fd)) != NULL) {
        struct wlr_egl* egl = wlr_gles2_renderer_get_egl(self->priv->wl_renderer);
        g_assert(egl != NULL);

        self->priv->renderer = expidus_runtime_compositor_egl_renderer_new(EXPIDUS_RUNTIME_COMPOSITOR_BACKEND(self), egl->display, egl->context);
        return expidus_runtime_compositor_wlroots_backend_renderered_init(self, cancellable, error);
      }
    }
#endif
  }

  g_debug("Using software rendering");
  if ((self->priv->wl_renderer = wlr_pixman_renderer_create()) == NULL) {
    expidus_runtime_compositor_wlroots_error_set_renderer(error, "failed to create a renderer");
    return FALSE;
  }
  return expidus_runtime_compositor_wlroots_backend_renderered_init(self, cancellable, error);
}

static const gchar* expidus_runtime_compositor_wlroots_backend_get_name(ExpidusRuntimeCompositorBackend* backend) {
  return "wlroots";
}

static ExpidusRuntimeCompositorRenderer* expidus_runtime_compositor_wlroots_backend_get_renderer(ExpidusRuntimeCompositorBackend* backend) {
  ExpidusRuntimeCompositorWlrootsBackend* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_BACKEND(backend);
  g_assert(self != NULL);
  return self->priv->renderer;
}

static void expidus_runtime_compositor_wlroots_backend_run(ExpidusRuntimeCompositorBackend* backend) {
  ExpidusRuntimeCompositorWlrootsBackend* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_BACKEND(backend);
  g_assert(self != NULL);

  g_debug("Running Wayland");
  wl_display_run(self->priv->display);
}

static GList* expidus_runtime_compositor_wlroots_backend_get_outputs(ExpidusRuntimeCompositorBackend* backend) {
  ExpidusRuntimeCompositorWlrootsBackend* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_BACKEND(backend);
  g_assert(self != NULL);
  return g_list_copy_deep(self->priv->outputs, (GCopyFunc)g_object_ref, NULL);
}

static void expidus_runtime_compositor_wlroots_backend_constructed(GObject* object) {
  G_OBJECT_CLASS(expidus_runtime_compositor_wlroots_backend_parent_class)->constructed(object);

  ExpidusRuntimeCompositorWlrootsBackend* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_BACKEND(object);
  g_assert(self != NULL);

  self->priv->display = wl_display_create();
  self->priv->event_loop = wl_display_get_event_loop(self->priv->display);
  self->priv->backend = wlr_backend_autocreate(self->priv->display);
}

static void expidus_runtime_compositor_wlroots_backend_dispose(GObject* object) {
  ExpidusRuntimeCompositorWlrootsBackend* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_BACKEND(object);
  g_assert(self != NULL);

  g_clear_object(&self->priv->renderer);
  g_clear_pointer(&self->priv->wl_renderer, wlr_renderer_destroy);
  g_clear_pointer(&self->priv->output_layout, wlr_output_layout_destroy);
  g_clear_pointer(&self->priv->backend, wlr_backend_destroy);
  g_clear_pointer(&self->priv->display, wl_display_destroy);

  G_OBJECT_CLASS(expidus_runtime_compositor_wlroots_backend_parent_class)->dispose(object);
}

static void expidus_runtime_compositor_wlroots_backend_finalize(GObject* object) {
  ExpidusRuntimeCompositorWlrootsBackend* self = EXPIDUS_RUNTIME_COMPOSITOR_WLROOTS_BACKEND(object);
  g_assert(self != NULL);

  G_OBJECT_CLASS(expidus_runtime_compositor_wlroots_backend_parent_class)->finalize(object);
}

static void expidus_runtime_compositor_wlroots_backend_log(enum wlr_log_importance verbosity, const char* fmt, va_list args) {
  GLogLevelFlags flags = 0;

  switch (verbosity) {
    case WLR_ERROR:
      flags = G_LOG_LEVEL_CRITICAL;
      break;
    case WLR_INFO:
      flags = G_LOG_LEVEL_INFO;
      break;
    case WLR_DEBUG:
      flags = G_LOG_LEVEL_DEBUG;
      break;
    default:
      return;
  }

  g_logv("wlroots", flags, fmt, args);
}

static void g_initable_interface_init(GInitableIface* interface) {
  interface->init = expidus_runtime_compositor_wlroots_backend_initable_init;
}

static void expidus_runtime_compositor_wlroots_backend_class_init(ExpidusRuntimeCompositorWlrootsBackendClass* klass) {
  ExpidusRuntimeCompositorBackendClass* backend_class = EXPIDUS_RUNTIME_COMPOSITOR_BACKEND_CLASS(klass);
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  backend_class->get_name = expidus_runtime_compositor_wlroots_backend_get_name;
  backend_class->get_renderer = expidus_runtime_compositor_wlroots_backend_get_renderer;
  backend_class->run = expidus_runtime_compositor_wlroots_backend_run;
  backend_class->get_outputs = expidus_runtime_compositor_wlroots_backend_get_outputs;

  object_class->constructed = expidus_runtime_compositor_wlroots_backend_constructed;
  object_class->dispose = expidus_runtime_compositor_wlroots_backend_dispose;
  object_class->finalize = expidus_runtime_compositor_wlroots_backend_finalize;
}

static void expidus_runtime_compositor_wlroots_backend_init(ExpidusRuntimeCompositorWlrootsBackend* self) {
  ExpidusRuntimeCompositorWlrootsBackendPrivate* priv = expidus_runtime_compositor_wlroots_backend_get_instance_private(self);
  g_assert(priv != NULL);
  self->priv = priv;
  priv->self = EXPIDUS_RUNTIME_COMPOSITOR_BACKEND(self);

  wlr_log_init(WLR_DEBUG, expidus_runtime_compositor_wlroots_backend_log);
}

ExpidusRuntimeCompositorBackend* expidus_runtime_compositor_wlroots_backend_new(GCancellable* cancellable, GError** error) {
  return g_initable_new(EXPIDUS_RUNTIME_COMPOSITOR_TYPE_WLROOTS_BACKEND, cancellable, error, NULL);
}
