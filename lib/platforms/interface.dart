import 'package:plugin_platform_interface/plugin_platform_interface.dart';
import 'method_channel.dart';

abstract class ExpidusPlatform extends PlatformInterface {
  ExpidusPlatform() : super(token: _token);

  static final Object _token = Object();

  static ExpidusRuntimePlatform _instance = ExpidusMethodChannel();
  static ExpidusRuntimePlatform get instance => _instance;

  static set instance(ExpidusRuntimePlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }
}
