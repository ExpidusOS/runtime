//
//  Generated file. Do not edit.
//

// clang-format off

#include "generated_plugin_registrant.h"

#include <expidus_runtime/expidus_runtime_plugin.h>

void fl_register_plugins(FlPluginRegistry* registry) {
  g_autoptr(FlPluginRegistrar) expidus_runtime_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "ExpidusRuntimePlugin");
  expidus_runtime_plugin_register_with_registrar(expidus_runtime_registrar);
}
