#include "include/expidus_runtime/expidus_runtime_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "expidus_runtime_plugin.h"

void ExpidusRuntimePluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  expidus_runtime::ExpidusRuntimePlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
