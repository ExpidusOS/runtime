{ pkgs ? import <expidus-sdk> {}, lib ? pkgs.lib }:
with lib;
pkgs.expidus.buildPackage {
  pname = "expidus-runtime-example";
  inherit (pkgs.expidus.runtime) version;
  src = cleanSource ./.;
  vendorSha256 = "sha256-McqX47XgOQAHBaDc2LjsXGiAjpX4Bc3ngsV1s2aM8qc=";
}
