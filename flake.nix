{
  description = "RetroEmu";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixpkgs-unstable";
  };

  outputs =
    { self, nixpkgs, ... }@inputs:
    let
      inherit (self) outputs;

      forAllSystems =
        function:
        nixpkgs.lib.genAttrs nixpkgs.lib.systems.flakeExposed (
          system: function nixpkgs.legacyPackages.${system}
        );
    in
    {
      formatter = forAllSystems (pkgs: pkgs.nixfmt);
      devShells = forAllSystems (pkgs: {
        default =
          let
            stdenv = pkgs.stdenvAdapters.useMoldLinker pkgs.gcc16Stdenv;
          in
          (pkgs.mkShell.override { inherit stdenv; }) {
            nativeBuildInputs = with pkgs; [
              nasm
              valgrind
              gdb

              clang-tools
              bear

              pkgconf
              cmake
              ninja

              SDL2
              SDL2_image
              SDL2_ttf

              skyemu
            ];
          };
      });
    };
}
