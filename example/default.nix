{ pkgs ? import <expidus-sdk> {}, lib ? pkgs.lib }:
with lib;
pkgs.expidus.buildPackage {
  pname = "expidus-runtime-example";
  inherit (pkgs.expidus.runtime) version;
  src = cleanSource ./.;
  vendorSha256 = "sha256-eHOcMcwSQfiDVYFcuWT1PYKFLjoh+0qiYUXmr86qjQ0=";
}
