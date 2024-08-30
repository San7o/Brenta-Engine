# Compile SDL3

Support for Alsa, Pipewire, Pulse and Oss.
Use this flake:
```nix
{
  description = "Build environment for the SDL3";

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
            libz                    # needed for assimp
            stdenv.cc.cc.lib        # libc
            cmake                   # build system
            alsa-lib.dev            # audio
            pulseaudio.dev
            pipewire.dev
            jack2.dev
            pkg-config
          ];
          shellHook = ''
              zsh
          '';

          LD_LIBRARY_PATH="${pkgsFor.${system}.libz}/lib:${pkgsFor.${system}.stdenv.cc.cc.lib}/lib:${pkgsFor.${system}.alsa-lib.dev}/lib:${pkgsFor.${system}.pulseaudio.dev}:${pkgsFor.${system}.pipewire.dev}:${pkgsFor.${system}.jack2.dev}";
        };
    });
  };
}

```

Run the following commands:
```bash
cmake -Bbuild -DSDL_OSS=ON -DSDL_PIPEWIRE=ON -DSDL_PIPEWIRE_SHARED=ON -DSDL_PULSEAUDIO=ON -DSDL_PULSEAUDIO_SHARED=ON -DSDL_ALSA=ON -DSDL_STATIC=ON -DSDL_ALSA_SHARED=ON

cmake --build build
```

# libassimp

Standard build

# libfreetype

Use this shell:
```bash
nix-shell -p libtool automake autoconf --run zsh
```
The first time you install, you need to remove the first command in `configure`

# libharfbuzz

Use cmake
