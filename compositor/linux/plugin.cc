#include "include/expidus_runtime_compositor/expidus_runtime_compositor_linux.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <sys/utsname.h>

#include <cstring>

#define EXPIDUS_RUNTIME_COMPOSITOR_LINUX(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), expidus_runtime_compositor_linux_get_type(), \
                              ExpidusRuntimeCompositorLinux))

struct _ExpidusRuntimeCompositorLinux {
  GObject parent_instance;
};

G_DEFINE_TYPE(ExpidusRuntimeCompositorLinux, expidus_runtime_compositor_linux, g_object_get_type())

// Called when a method call is received from Flutter.
static void expidus_runtime_compositor_linux_handle_method_call(
    ExpidusRuntimeCompositorLinux* self,
    FlMethodCall* method_call) {
  g_autoptr(FlMethodResponse) response = nullptr;

  // const gchar* method = fl_method_call_get_name(method_call);

  response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());

  fl_method_call_respond(method_call, response, nullptr);
}

static void expidus_runtime_compositor_linux_dispose(GObject* object) {
  G_OBJECT_CLASS(expidus_runtime_compositor_linux_parent_class)->dispose(object);
}

static void expidus_runtime_compositor_linux_class_init(ExpidusRuntimeCompositorLinuxClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = expidus_runtime_compositor_linux_dispose;
}

static void expidus_runtime_compositor_linux_init(ExpidusRuntimeCompositorLinux* self) {}

static void method_call_cb(FlMethodChannel* channel, FlMethodCall* method_call,
                           gpointer user_data) {
  ExpidusRuntimeCompositorLinux* plugin = EXPIDUS_RUNTIME_COMPOSITOR_LINUX(user_data);
  expidus_runtime_compositor_linux_handle_method_call(plugin, method_call);
}

void expidus_runtime_compositor_linux_register_with_registrar(FlPluginRegistrar* registrar) {
  ExpidusRuntimeCompositorLinux* plugin = EXPIDUS_RUNTIME_COMPOSITOR_LINUX(
      g_object_new(expidus_runtime_compositor_linux_get_type(), nullptr));

  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  g_autoptr(FlMethodChannel) channel =
      fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar),
                            "expidus_runtime_compositor",
                            FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(channel, method_call_cb,
                                            g_object_ref(plugin),
                                            g_object_unref);

  g_object_unref(plugin);
}
