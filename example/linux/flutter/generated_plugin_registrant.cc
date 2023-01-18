//
//  Generated file. Do not edit.
//

// clang-format off

#include "generated_plugin_registrant.h"

#include <expidus_runtime_compositor/expidus_runtime_compositor_linux.h>

void fl_register_plugins(FlPluginRegistry* registry) {
  g_autoptr(FlPluginRegistrar) expidus_runtime_compositor_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "ExpidusRuntimeCompositorLinux");
  expidus_runtime_compositor_linux_register_with_registrar(expidus_runtime_compositor_registrar);
}
