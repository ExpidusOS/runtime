import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';
import 'interface.dart';

class ExpidusRuntimeCompositorMethodChannel extends ExpidusRuntimeCompositorPlatform {
  @visibleForTesting
  final methodChannel = const MethodChannel('expidus_runtime_compositor');
}
