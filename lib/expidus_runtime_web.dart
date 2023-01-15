// In order to *not* need this ignore, consider extracting the "web" version
// of your plugin as a separate package, instead of inlining it in the same
// package as the core of your plugin.
// ignore: avoid_web_libraries_in_flutter
import 'dart:html' as html show window;

import 'package:flutter_web_plugins/flutter_web_plugins.dart';

import 'expidus_runtime_platform_interface.dart';

/// A web implementation of the ExpidusRuntimePlatform of the ExpidusRuntime plugin.
class ExpidusRuntimeWeb extends ExpidusRuntimePlatform {
  /// Constructs a ExpidusRuntimeWeb
  ExpidusRuntimeWeb();

  static void registerWith(Registrar registrar) {
    ExpidusRuntimePlatform.instance = ExpidusRuntimeWeb();
  }

  /// Returns a [String] containing the version of the platform.
  @override
  Future<String?> getPlatformVersion() async {
    final version = html.window.navigator.userAgent;
    return version;
  }
}
