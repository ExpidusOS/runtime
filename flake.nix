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
            buildInputs = old.buildInputs ++ (with pkgs; [ libdrm ]);
            src = cleanSourceWith {
              filter = name: type:
                let
                  baseName = baseNameOf (toString name);
                in baseName != "flake.nix" && baseName != "flake.lock";
              src = cleanSource self;
            };
          });

          runtime-example = import ./example/default.nix { inherit pkgs; };
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

          mkffigen-config = file:
            pkgs.writeTextFile {
              name = "ffigen.yaml";
              text = ''
                ${readFile file}
                  - '-I${pkgs.clang14Stdenv.cc.libc.dev}/include -I${pkgs.gcc-unwrapped}/lib/gcc/${pkgs.targetPlatform.config}/${pkgs.gcc.version}/include -I${pkgs.clang14Stdenv.cc.libc.dev.linuxHeaders}/include'
                llvm-path:
                  - ${pkgs.llvmPackages_14.llvm}
                  - ${pkgs.llvmPackages_14.llvm.dev}
                  - ${pkgs.llvmPackages_14.llvm.lib}
                  - ${pkgs.llvmPackages_14.libclang}
                  - ${pkgs.llvmPackages_14.libclang.dev}
                  - ${pkgs.llvmPackages_14.libclang.lib}
              '';
            };

          mkffigen = file: name: pkgs.writeShellScriptBin name ''
            flutter pub run ffigen --config ${mkffigen-config file}
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
              (mkffigen ./ffigen.yaml "ffigen")
            ];
          };
        });
    };
}
