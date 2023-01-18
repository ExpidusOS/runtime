#include <expidus/runtime/compositor.h>
#include <expidus/runtime/compositor/output.h>
#include <glib.h>
#include "compositor-priv.h"

G_DEFINE_TYPE_WITH_PRIVATE(ExpidusRuntimeCompositor, expidus_runtime_compositor, G_TYPE_APPLICATION);

enum {
  PROP_BINARY_NAME = 1,
  PROP_BACKEND,
  N_PROPERTIES,
};

static GParamSpec* obj_properties[N_PROPERTIES] = { NULL };

static void expidus_runtime_compositor_log(const char* tag, const char* message, void* user_data) {
  g_log("Dart", G_LOG_LEVEL_INFO, "%s: %s", tag, message);
}

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

  g_clear_pointer(&self->priv->argv0, g_free);
  g_clear_pointer(&self->priv->binary_name, g_free);

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

  expidus_runtime_compositor_backend_set_compositor(self->priv->backend, self);

  if (self->priv->arguments.backend == NULL) {
    g_debug("Compositor is using the %s display server backend.", expidus_runtime_compositor_backend_get_name(self->priv->backend));
  }

  const gchar* dir = g_path_get_dirname(self->priv->argv0);

  self->priv->project_args.assets_path = g_build_filename(dir, "data", "flutter_assets", NULL);
  self->priv->project_args.icu_data_path = g_build_filename(dir, "data", "icudtl.dat", NULL);

  self->priv->flutter_procs.struct_size = sizeof (FlutterEngineProcTable);
  FlutterEngineResult result = FlutterEngineGetProcAddresses(&self->priv->flutter_procs);
  if (result != kSuccess) {
    g_error("Failed to get Flutter proc addresses");
  }

  if (self->priv->flutter_procs.RunsAOTCompiledDartCode()) {
    g_debug("Flutter will be using AOT");

    FlutterEngineAOTDataSource aot_source = (FlutterEngineAOTDataSource) {
      .elf_path = g_build_filename(dir, "lib", "libapp.so", NULL),
      .type = kFlutterEngineAOTDataSourceTypeElfPath
    };

    result = FlutterEngineCreateAOTData(&aot_source, &self->priv->project_args.aot_data);
    if (result != kSuccess) {
      g_error("Failed to create AOT data");
    }
  } else {
    g_debug("Flutter will not be using AOT");
  }

  ExpidusRuntimeCompositorRenderer* renderer = expidus_runtime_compositor_backend_get_renderer(self->priv->backend);
  g_assert(renderer != NULL);

  FlutterRendererConfig* renderer_config = expidus_runtime_compositor_renderer_get_config(renderer);
  g_assert(renderer_config != NULL);

  self->priv->project_args.log_message_callback = expidus_runtime_compositor_log;
  self->priv->project_args.compositor = expidus_runtime_compositor_renderer_get_compositor(renderer);

  result = self->priv->flutter_procs.Run(FLUTTER_ENGINE_VERSION, renderer_config, &self->priv->project_args, self, &self->priv->engine);
  if (result != kSuccess) {
    g_error("Failed to launch Flutter");
  }

  result = expidus_runtime_compositor_backend_display_update(self, kFlutterEngineDisplaysUpdateTypeStartup);
  if (result != kSuccess) {
    g_error("Failed to submit displays to Flutter");
  }

  expidus_runtime_compositor_backend_run(self->priv->backend);
}

static int expidus_runtime_compositor_command_line(GApplication* application, GApplicationCommandLine* cmdline) {
  ExpidusRuntimeCompositor* self = EXPIDUS_RUNTIME_COMPOSITOR(application);

  int argc = 0;
  gchar** argv = g_application_command_line_get_arguments(cmdline, &argc);
  self->priv->argv0 = g_strdup(argv[0]);
  self->priv->project_args.command_line_argv = (const char**)g_strdupv(argv + 1);
  self->priv->project_args.command_line_argc = argc - 1;

  GError* error = NULL;
  if (!g_application_register(application, NULL, &error)) {
    g_error("Failed to register: %s", error->message);
    return FALSE;
  }

  g_application_activate(application);
  return 1;
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
  application_class->command_line = expidus_runtime_compositor_command_line;

  obj_properties[PROP_BINARY_NAME] = g_param_spec_string("binary-name", "Binary name", "The name of the binary being executed", NULL, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  obj_properties[PROP_BACKEND] = g_param_spec_object("backend", "Backend", "The display system backend instance", EXPIDUS_RUNTIME_COMPOSITOR_TYPE_BACKEND, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}

static void expidus_runtime_compositor_init(ExpidusRuntimeCompositor* self) {
  ExpidusRuntimeCompositorPrivate* priv = expidus_runtime_compositor_get_instance_private(self);
  g_assert(priv != NULL);

  self->priv = priv;
  self->priv->project_args.struct_size = sizeof (FlutterProjectArgs);
}

ExpidusRuntimeCompositor* expidus_runtime_compositor_new(const gchar* application_id, const gchar* binary_name) {
  return g_object_new(EXPIDUS_RUNTIME_TYPE_COMPOSITOR, "application-id", application_id, "binary-name", binary_name, "flags", G_APPLICATION_HANDLES_COMMAND_LINE, NULL);
}

ExpidusRuntimeCompositor* expidus_runtime_compositor_new_with_backend(ExpidusRuntimeCompositorBackend* backend, const gchar* application_id, const gchar* binary_name) {
  return g_object_new(EXPIDUS_RUNTIME_TYPE_COMPOSITOR, "backend", NULL, "application-id", application_id, "binary-name", binary_name, "flags", G_APPLICATION_HANDLES_COMMAND_LINE, NULL);
}

ExpidusRuntimeCompositorBackend* expidus_runtime_compositor_get_backend(ExpidusRuntimeCompositor* self) {
  g_return_val_if_fail(EXPIDUS_RUNTIME_IS_COMPOSITOR(self), NULL);
  return self->priv->backend;
}

FlutterEngine* expidus_runtime_compositor_get_engine(ExpidusRuntimeCompositor* self) {
  g_return_val_if_fail(EXPIDUS_RUNTIME_IS_COMPOSITOR(self), NULL);
  return &self->priv->engine;
}

FlutterEngineResult expidus_runtime_compositor_backend_display_update(ExpidusRuntimeCompositor* self, FlutterEngineDisplaysUpdateType update_type) {
  GList* outputs = expidus_runtime_compositor_backend_get_outputs(self->priv->backend);
  g_assert(outputs != NULL);

  guint n_outputs = g_list_length(outputs);
  g_assert(n_outputs > 0);

  FlutterEngineDisplay* displays = g_malloc(sizeof (FlutterEngineDisplay) * n_outputs);

  size_t i = 0;
  for (GList* item = outputs; item != NULL; item = item->next) {
    ExpidusRuntimeCompositorOutput* output = EXPIDUS_RUNTIME_COMPOSITOR_OUTPUT(item->data);
    FlutterEngineDisplay* display = expidus_runtime_compositor_output_get_engine(output);
    g_assert(display != NULL);

    display->single_display = n_outputs == 0;

    memcpy(&displays[i++], display, sizeof (FlutterEngineDisplay));
  }

  FlutterEngineResult result = FlutterEngineNotifyDisplayUpdate(self->priv->engine, update_type, displays, n_outputs);
  g_list_free_full(outputs, g_object_unref);
  return result;
}

void expidus_runtime_compositor_scene_layers_init(ExpidusRuntimeCompositor* self, ExpidusRuntimeCompositorScene* scene, const FlutterLayer* layer) {
  g_return_if_fail(EXPIDUS_RUNTIME_IS_COMPOSITOR(self));
  g_return_if_fail(EXPIDUS_RUNTIME_COMPOSITOR_IS_SCENE(scene));
}
