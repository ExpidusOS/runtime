{
  description = "The runtime environment for ExpidusOS";

  inputs.expidus-sdk.url = github:ExpidusOS/sdk/refactor;

  outputs = { self, expidus-sdk }:
    with expidus-sdk.lib;
    let
      defaultOverlay = pkgs: prev: {
        expidus = prev.expidus // {
          runtime = prev.expidus.runtime.overrideAttrs (old: {
            version = self.shortRev or "dirty";
            src = cleanSource self;
          });

          runtime-example = prev.expidus.runtime-example.overrideAttrs (old: {
            inherit (pkgs.expidus) runtime;
            src = old.src.overrideAttrs (old: {
              src = "${pkgs.expidus.runtime}/example";
            });
            vendorSha256 = "sha256-qOxKTrHRPO0LHMI8gdwUGFLBrjnhymImhVs7djWi2sA=";
          });
        };
      };

      legacyPackages = mapAttrs (system: pkgs: pkgs.appendOverlays ([
        defaultOverlay
      ])) expidus-sdk.legacyPackages;
    in {
      overlays.default = defaultOverlay;
      inherit legacyPackages;

      packages = expidus.system.default.forAllSystems (system: localSystem:
        let
          pkgs = legacyPackages.${system};
        in {
          default = pkgs.expidus.runtime;
          example = pkgs.expidus.runtime-example;
        });

      devShells = expidus.system.default.forAllSystems (system: localSystem:
        let
          pkgs = legacyPackages.${system};

          ffigen-config = pkgs.writeTextFile {
            name = "ffigen.yaml";
            text = ''
              ${readFile ./ffigen.yaml}
              llvm-path:
                - ${pkgs.llvmPackages_14.llvm}
                - ${pkgs.llvmPackages_14.llvm.dev}
                - ${pkgs.llvmPackages_14.llvm.lib}
                - ${pkgs.llvmPackages_14.libclang}
                - ${pkgs.llvmPackages_14.libclang.dev}
                - ${pkgs.llvmPackages_14.libclang.lib}
            '';
          };

          ffigen = pkgs.writeShellScriptBin "ffigen" ''
            flutter pub run ffigen --config ${ffigen-config}
          '';
        in {
          default = pkgs.mkShell { 
            name = "${pkgs.expidus.runtime.name}-shell";

            inherit (pkgs.expidus.runtime-example) LD_LIBRARY_PATH;

            packages = with pkgs.expidus; [
              runtime.buildInputs
              runtime.nativeBuildInputs
              runtime-example.buildInputs
              runtime-example.nativeBuildInputs
              ffigen
            ];
          };
        });
    };
}
