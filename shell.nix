{
  pkgs ? import <nixpkgs> {}
}:

let
  stdenv = pkgs.stdenvAdapters.useMoldLinker pkgs.llvmPackages_22.stdenv;
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
}
