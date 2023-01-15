import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:expidus_runtime/expidus_runtime_method_channel.dart';

void main() {
  MethodChannelExpidusRuntime platform = MethodChannelExpidusRuntime();
  const MethodChannel channel = MethodChannel('expidus_runtime');

  TestWidgetsFlutterBinding.ensureInitialized();

  setUp(() {
    channel.setMockMethodCallHandler((MethodCall methodCall) async {
      return '42';
    });
  });

  tearDown(() {
    channel.setMockMethodCallHandler(null);
  });

  test('getPlatformVersion', () async {
    expect(await platform.getPlatformVersion(), '42');
  });
}
