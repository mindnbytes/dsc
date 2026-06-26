{
  description = "Educational C data structures project";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-26.05";

  outputs =
    { nixpkgs, ... }:
    let
      systems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];

      forAllSystems = nixpkgs.lib.genAttrs systems;
    in
    {
      devShells = forAllSystems (
        system:
        let
          pkgs = import nixpkgs { inherit system; };
        in
        {
          default = pkgs.mkShell {
            packages =
              with pkgs;
              [
                clang
                clang-tools
                gnumake
                lldb
              ]
              ++ pkgs.lib.optionals pkgs.stdenv.isLinux [
                valgrind
                gdb
              ];

            shellHook = ''
              echo "C dev shell: clang, make, clang-tools"
            '';
          };
        }
      );
    };
}
