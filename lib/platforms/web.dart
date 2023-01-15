import 'dart:html' as html show window;
import 'package:flutter_web_plugins/flutter_web_plugins.dart';
import 'interface.dart';

class ExpidusWebPlatform extends ExpidusPlatform {
  ExpidusWebPlatform();

  static void registerWith(Registrar registrar) {
    ExpidusPlatform.instance = ExpidusWebPlatform();
  }
}
