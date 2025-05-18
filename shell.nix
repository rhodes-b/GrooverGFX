{ pkgs ? import <nixpkgs> {}, pkgs-unstable ? import <nixpkgs-unstable> {} }:
  pkgs.mkShell {
    nativeBuildInputs = [
      pkgs.gdb
      pkgs-unstable.zig
      pkgs-unstable.zls
    ];
}
