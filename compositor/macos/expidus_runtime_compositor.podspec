#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html.
# Run `pod lib lint expidus_runtime_compositor.podspec` to validate before publishing.
#
Pod::Spec.new do |s|
  s.name             = 'expidus_runtime_compositor'
  s.version          = '0.0.1'
  s.summary          = 'Compositing related section of the ExpidusOS runtime.'
  s.description      = <<-DESC
Compositing related section of the ExpidusOS runtime.
                       DESC
  s.homepage         = 'https://expidusos.com'
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'Midstall Software' => 'inquiry@midstall.com' }

  s.source           = { :path => '.' }
  s.source_files     = 'Classes/**/*'
  s.dependency 'FlutterMacOS'

  s.platform = :osx, '10.11'
  s.pod_target_xcconfig = { 'DEFINES_MODULE' => 'YES' }
  s.swift_version = '5.0'
end
