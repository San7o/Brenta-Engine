{
  description = "Development environment for the engine";

  inputs = {
    nixpkgs = { url = "github:nixos/nixpkgs/nixos-unstable"; };
  };

  outputs = { self, nixpkgs }: 
  let 
    supportedSystems = [ "x86_64-linux" "aarch64-linux" ];
    
    # Helper function to generate an attrset '{ x86_64-linux = f "x86_64-linux"; ... }'.
    forAllSystems = nixpkgs.lib.genAttrs supportedSystems;

    # Nixpkgs instantiated for supported system types.
    pkgsFor = forAllSystems (system: import nixpkgs { inherit system; });
  in
  {
    devShells = forAllSystems (system: {
      default =
        pkgsFor.${system}.mkShell.override {
          stdenv = pkgsFor.${system}.gcc14Stdenv;
        } {

          name = "engine-dev-shell";
          hardeningDisable = ["all"];
          packages = with pkgsFor.${system}; [
            libz.dev                # needed for assimp
            stdenv.cc.cc.lib        # libc
            cmake                   # build system
            glfw-wayland-minecraft  # OpenGL windowing library
            freetype                # font rendering
            harfbuzzFull            # text shaping
            alsa-lib.dev            # audio
            doxygen                 # documentation
            clang-tools             # code formatting
          ];
          shellHook = ''
              zsh
          '';

          LD_LIBRARY_PATH="${pkgsFor.${system}.libz.dev}:${pkgsFor.${system}.alsa-lib.dev}";
        };
    });
  };
}
