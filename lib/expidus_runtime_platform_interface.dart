import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'expidus_runtime_method_channel.dart';

abstract class ExpidusRuntimePlatform extends PlatformInterface {
  /// Constructs a ExpidusRuntimePlatform.
  ExpidusRuntimePlatform() : super(token: _token);

  static final Object _token = Object();

  static ExpidusRuntimePlatform _instance = MethodChannelExpidusRuntime();

  /// The default instance of [ExpidusRuntimePlatform] to use.
  ///
  /// Defaults to [MethodChannelExpidusRuntime].
  static ExpidusRuntimePlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [ExpidusRuntimePlatform] when
  /// they register themselves.
  static set instance(ExpidusRuntimePlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<String?> getPlatformVersion() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }
}
