{ pkgs ? import <expidus-sdk> {}, lib ? pkgs.lib }:
with lib;
pkgs.expidus.buildPackage {
  pname = "expidus-runtime-example";
  inherit (pkgs.expidus.runtime) version;
  src = cleanSource "${pkgs.expidus.runtime.src}/example";
  vendorSha256 = "sha256-WD5jCuO6M127gUJUandhZbhkYZZhWgDJTao51hUS1/c=";
}
