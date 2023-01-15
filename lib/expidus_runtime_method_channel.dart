import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'expidus_runtime_platform_interface.dart';

/// An implementation of [ExpidusRuntimePlatform] that uses method channels.
class MethodChannelExpidusRuntime extends ExpidusRuntimePlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('expidus_runtime');

  @override
  Future<String?> getPlatformVersion() async {
    final version = await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }
}
