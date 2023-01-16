{ pkgs ? import <expidus-sdk> {}, lib ? pkgs.lib }:
with lib;
pkgs.expidus.buildPackage {
  pname = "expidus-runtime-example";
  inherit (pkgs.expidus.runtime) version;
  src = cleanSource "${pkgs.expidus.runtime.src}/example";
  vendorSha256 = "sha256-gK7WkV85AiLrVuB/SdzdRMVT8okTbGvkQdVkSbFDImE=";
}
