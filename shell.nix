{ pkgs ? import <nixpkgs> {}
}: pkgs.mkShell {
    nativeBuildInputs = with pkgs.buildPackages; [
      cmake
      python3
      gdb
      cloc
      valgrind
    ];
}
