#include <expidus/runtime/compositor.h>
#include <glib.h>
#include "compositor-priv.h"

G_DEFINE_TYPE_WITH_PRIVATE(ExpidusRuntimeCompositor, expidus_runtime_compositor, G_TYPE_APPLICATION);

enum {
  PROP_BINARY_NAME = 1,
  PROP_BACKEND,
  N_PROPERTIES,
};

static GParamSpec* obj_properties[N_PROPERTIES] = { NULL };

static void expidus_runtime_compositor_constructed(GObject* object) {
  G_OBJECT_CLASS(expidus_runtime_compositor_parent_class)->constructed(object);

  ExpidusRuntimeCompositor* self = EXPIDUS_RUNTIME_COMPOSITOR(object);

  GOptionGroup* optgroup = g_option_group_new("expidus-runtime-compositor", "ExpidusOS Runtime - Compositor", "Options for the compositor side of the ExpidusOS Runtime", NULL, NULL);

  const GOptionEntry options[] = {
    { "expidus-runtime-compositor-backend", 0, G_OPTION_FLAG_NONE, G_OPTION_ARG_STRING, &self->priv->arguments.backend, "Set the backend to utilize", NULL },
    { NULL }
  };

  g_option_group_add_entries(optgroup, options);
  g_application_add_option_group(G_APPLICATION(object), optgroup);
}

static void expidus_runtime_compositor_dispose(GObject* object) {
  ExpidusRuntimeCompositor* self = EXPIDUS_RUNTIME_COMPOSITOR(object);

  g_clear_object(&self->priv->backend);

  G_OBJECT_CLASS(expidus_runtime_compositor_parent_class)->dispose(object);
}

static void expidus_runtime_compositor_finalize(GObject* object) {
  ExpidusRuntimeCompositor* self = EXPIDUS_RUNTIME_COMPOSITOR(object);

  g_free(self->priv->binary_name);

  G_OBJECT_CLASS(expidus_runtime_compositor_parent_class)->finalize(object);
}

static void expidus_runtime_compositor_get_property(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec) {
  ExpidusRuntimeCompositor* self = EXPIDUS_RUNTIME_COMPOSITOR(object);

  switch (prop_id) {
    case PROP_BINARY_NAME:
      g_value_set_string(value, self->priv->binary_name);
      break;
    case PROP_BACKEND:
      g_value_set_object(value, self->priv->backend);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
      break;
  }
}

static void expidus_runtime_compositor_set_property(GObject* object, guint prop_id, const GValue* value, GParamSpec* pspec) {
  ExpidusRuntimeCompositor* self = EXPIDUS_RUNTIME_COMPOSITOR(object);

  switch (prop_id) {
    case PROP_BINARY_NAME:
      self->priv->binary_name = g_value_dup_string(value);
      break;
    case PROP_BACKEND:
      self->priv->backend = EXPIDUS_RUNTIME_COMPOSITOR_BACKEND(g_value_dup_object(value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
      break;
  }
}

static void expidus_runtime_compositor_activate(GApplication* application) {
  ExpidusRuntimeCompositor* self = EXPIDUS_RUNTIME_COMPOSITOR(application);
  g_assert(self != NULL);

  GError* error = NULL;

  if (self->priv->backend == NULL) {
    if (self->priv->arguments.backend == NULL) {
      g_debug("Compositor will automatically determine the display server backend.");
      self->priv->backend = expidus_runtime_compositor_backend_new_auto(NULL, &error);
    } else {
      g_debug("Compositor will use the %s display server backend.", self->priv->arguments.backend);
      self->priv->backend = expidus_runtime_compositor_backend_new(self->priv->arguments.backend, NULL, &error);
    }
  }

  g_assert(self->priv->backend != NULL);
  if (self->priv->arguments.backend == NULL) {
    g_debug("Compositor is using the %s display server backend.", expidus_runtime_compositor_backend_get_name(self->priv->backend));
  }

  // TODO: initialize and start flutter

  expidus_runtime_compositor_backend_run(self->priv->backend);
}

static void expidus_runtime_compositor_class_init(ExpidusRuntimeCompositorClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);
  GApplicationClass* application_class = G_APPLICATION_CLASS(klass);
  
  object_class->constructed = expidus_runtime_compositor_constructed;
  object_class->dispose = expidus_runtime_compositor_dispose;
  object_class->finalize = expidus_runtime_compositor_finalize;
  object_class->get_property = expidus_runtime_compositor_get_property;
  object_class->set_property = expidus_runtime_compositor_set_property;

  application_class->activate = expidus_runtime_compositor_activate;

  obj_properties[PROP_BINARY_NAME] = g_param_spec_string("binary-name", "Binary name", "The name of the binary being executed", NULL, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  obj_properties[PROP_BACKEND] = g_param_spec_object("backend", "Backend", "The display system backend instance", EXPIDUS_RUNTIME_COMPOSITOR_TYPE_BACKEND, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}

static void expidus_runtime_compositor_init(ExpidusRuntimeCompositor* self) {
  ExpidusRuntimeCompositorPrivate* priv = expidus_runtime_compositor_get_instance_private(self);
  g_assert(priv != NULL);
  self->priv = priv;
}

ExpidusRuntimeCompositor* expidus_runtime_compositor_new(const gchar* application_id, const gchar* binary_name) {
  return g_object_new(EXPIDUS_RUNTIME_TYPE_COMPOSITOR, "application-id", application_id, "binary-name", binary_name, NULL);
}

ExpidusRuntimeCompositor* expidus_runtime_compositor_new_with_backend(ExpidusRuntimeCompositorBackend* backend, const gchar* application_id, const gchar* binary_name) {
  return g_object_new(EXPIDUS_RUNTIME_TYPE_COMPOSITOR, "backend", NULL, "application-id", application_id, "binary-name", binary_name, NULL);
}
