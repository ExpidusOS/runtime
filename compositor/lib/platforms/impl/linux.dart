import '../interface.dart';

class ExpidusRuntimeCompositorLinux extends ExpidusRuntimeCompositorPlatform {
  ExpidusRuntimeCompositorLinux();

  static void registerWith(Registrar registrar) {
    ExpidusRuntimeCompositorPlatform.instance = ExpidusRuntimeCompositorLinux();
  }
}
