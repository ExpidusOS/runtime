import 'package:flutter_test/flutter_test.dart';
import 'package:expidus_runtime/expidus_runtime.dart';
import 'package:expidus_runtime/expidus_runtime_platform_interface.dart';
import 'package:expidus_runtime/expidus_runtime_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockExpidusRuntimePlatform
    with MockPlatformInterfaceMixin
    implements ExpidusRuntimePlatform {

  @override
  Future<String?> getPlatformVersion() => Future.value('42');
}

void main() {
  final ExpidusRuntimePlatform initialPlatform = ExpidusRuntimePlatform.instance;

  test('$MethodChannelExpidusRuntime is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelExpidusRuntime>());
  });

  test('getPlatformVersion', () async {
    ExpidusRuntime expidusRuntimePlugin = ExpidusRuntime();
    MockExpidusRuntimePlatform fakePlatform = MockExpidusRuntimePlatform();
    ExpidusRuntimePlatform.instance = fakePlatform;

    expect(await expidusRuntimePlugin.getPlatformVersion(), '42');
  });
}
