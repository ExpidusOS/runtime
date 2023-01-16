import 'dart:html' as html show window;
import 'package:flutter_web_plugins/flutter_web_plugins.dart';
import '../interface.dart';

class ExpidusRuntimeCompositorWeb extends ExpidusRuntimeCompositorPlatform {
  ExpidusRuntimeCompositorWeb();

  static void registerWith(Registrar registrar) {
    ExpidusRuntimeCompositorPlatform.instance = ExpidusRuntimeCompositorWeb();
  }
}
