{ pkgs ? import <expidus-sdk> {} }:
pkgs.expidus.buildPackage {
  pname = "expidus-runtime-example";
  inherit (pkgs.expidus.runtime) version;
  src = ./.;
  vendorSha256 = "sha256-gK7WkV85AiLrVuB/SdzdRMVT8okTbGvkQdVkSbFDImE=";
}
