
import 'expidus_runtime_platform_interface.dart';

class ExpidusRuntime {
  Future<String?> getPlatformVersion() {
    return ExpidusRuntimePlatform.instance.getPlatformVersion();
  }
}
