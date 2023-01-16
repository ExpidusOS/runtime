#include "include/expidus_runtime_compositor/expidus_runtime_compositor_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "expidus_runtime_compositor_plugin.h"

void ExpidusRuntimeCompositorPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  expidus_runtime_compositor::ExpidusRuntimeCompositorPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
