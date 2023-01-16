#ifndef FLUTTER_PLUGIN_EXPIDUS_RUNTIME_COMPOSITOR_PLUGIN_H_
#define FLUTTER_PLUGIN_EXPIDUS_RUNTIME_COMPOSITOR_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>

namespace expidus_runtime_compositor {

class ExpidusRuntimeCompositorPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  ExpidusRuntimeCompositorPlugin();

  virtual ~ExpidusRuntimeCompositorPlugin();

  // Disallow copy and assign.
  ExpidusRuntimeCompositorPlugin(const ExpidusRuntimeCompositorPlugin&) = delete;
  ExpidusRuntimeCompositorPlugin& operator=(const ExpidusRuntimeCompositorPlugin&) = delete;

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
};

}  // namespace expidus_runtime_compositor

#endif  // FLUTTER_PLUGIN_EXPIDUS_RUNTIME_COMPOSITOR_PLUGIN_H_
