import Cocoa
import FlutterMacOS

public class ExpidusRuntimeCompositorPlugin: NSObject, FlutterPlugin {
  public static func register(with registrar: FlutterPluginRegistrar) {
    let channel = FlutterMethodChannel(name: "expidus_runtime_compositor", binaryMessenger: registrar.messenger)
    let instance = ExpidusRuntimeCompositorPlugin()
    registrar.addMethodCallDelegate(instance, channel: channel)
  }

  public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
    result(FlutterMethodNotImplemented)
  }
}
