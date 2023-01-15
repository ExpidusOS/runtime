import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';
import 'interface.dart';

class ExpidusMethodChannel extends ExpidusPlatform {
  @visibleForTesting
  final methodChannel = const MethodChannel('expidus');
}
