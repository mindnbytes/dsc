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
          llvm = pkgs.llvmPackages_22;
        in
        {
          default = pkgs.mkShell {
            packages =
              with pkgs;
              [
                llvm.clang
                llvm.clang-tools
                gnumake
                llvm.lldb
              ]
              ++ pkgs.lib.optionals pkgs.stdenv.isLinux [
                valgrind
                gdb
              ];
            shellHook = ''
              export CC=${llvm.clang}/bin/clang
              echo "C dev shell: clang 22, make, clang-tools"
              clang --version | head -n 1
            '';
          };
        }
      );
    };
}
