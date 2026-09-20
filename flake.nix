{
  description = "Set of Core Utils for c++ projects";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachSystem
      [
        "x86_64-linux"
        "i686-linux"
        "aarch64-linux"
        "x86_64-darwin"
      ]
      (
        system:
        let
          pkgs = import nixpkgs {
            inherit system;
            overlays = [ ];
          };
        in
        {
          devShells.default = pkgs.mkShell rec {
            name = "cpp-core-utils";

            packages = with pkgs; [
              gcc
              cmake
              ninja
              clang-tools
            ];

            shellHook = ''
              exec zsh
              echo "cpp-core-utils env ready"
            '';
          };
        }
      );
}
