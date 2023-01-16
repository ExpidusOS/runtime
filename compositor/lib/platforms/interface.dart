import 'package:plugin_platform_interface/plugin_platform_interface.dart';
import 'method_channel.dart';

abstract class ExpidusRuntimeCompositorPlatform extends PlatformInterface {
  ExpidusRuntimeCompositorPlatform(): super(token: _token);

  static final Object _token = Object();

  static ExpidusRuntimeCompositorPlatform _instance = ExpidusRuntimeCompositorMethodChannel();
  static ExpidusRuntimeCompositorPlatform get instance => _instance;

  static set instance(ExpidusRuntimeCompositorPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = _instance;
  }
}
