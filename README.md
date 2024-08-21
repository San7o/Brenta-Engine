# opengl-santo-engine

A friendly Entity Component System (ECS) engine, 
developed in C++ with OpenGL. This engine is designed to 
provide a modular and flexible architecture for game development
and simulations: you can run simultations without the graphical
interface through the custom made - from the ground up - ECS framework.
Or you could build any graphical application with OpenGL: the engine is
packed with easy to use and highly modular and customizable classes 
to handle most necessities like Input, 3D Model Loading, Audio, Time...

Check out `GUIDE.md` for an introduction on how the system works!



https://github.com/user-attachments/assets/f0ea502c-dc9e-4609-8322-641eb7d65a77
> Model loading, physics and lighting!


# ⭐ Current State

The project is structured into the folders `engine`, `ecs`, `game`, `render` 
containing respectively the game engine, the ecs, a game demo and a render demo.

The project is under heavy developement, the following features have been
implemented:

- [x] Triangles

- [x] Textures

- [x] Camera

- [x] Shaders 

- [x] Lighting: Ambient, Diffuse, Specular

- [x] Model loading

- [x] Other OpenGL features: Texture transparency, Face culling, MSAA

- [x] ECS: Entities, Components, Systems, Resources, World, Query multiple components

- [x] Unit tests

- [x] Logging

- [x] Text engine

- [x] Audio engine

- [x] Simple Physics

- [x] Collisions: Spheres

- [x] Texture Atlas

- [x] Particles

Checkout [CONTRIBUTING](./CONTRIBUTING.md) if you are considering contributing to the project.

# 💀 Dependencies

You need:

- A `C++17` compiler

- `GLFW`

- `libfreetype`

- `libassimp`

- `SDL3` for audio, make sure you compile for your audio driver

- `doxygen` for building documentation

If you are on `Nix`, you can access the developement environment
by running:
```bash
nix develop
```

You need [Assimp](https://github.com/assimp/assimp) in order to import 3D models. It is
advised to build the library for your platform following the instruction in the official
repository, but many linux distributions have it available in their packet manager. 
For linux, I built the library from source using the following flags:
```bash
git clone https://github.com/assimp/assimp.git
cd assimp/
cmake CMakeLists.txt -DASSIMP_WARNINGS_AS_ERRORS=off -DASSIMP_INSTALL=off
make -j4
```
The library will be generated in `lib/`. If you don't install the library system-wide,
you need to specify the path in an environment variable to run the application, like so:
```bash
LD_LIBRARY_PATH=${PWD}/lib/:${LD_LIBRARY_PATH} ./build/main.out
```
Or you could compile the library statically with `-static`, but It will take more compile
time and generate a bigger file.

Likewise, you should compile your libraries for [FreeType](https://freetype.org/) and [SDL3](https://wiki.libsdl.org/SDL3/FrontPage).

# 🪖 Compile

You can compile the demo game with `cmake` (recommended):
```bash
cmake -Bbuild
cmake --build build
./build/main
```

Or with `make`:
```bash
# Demo game using the engine
make game
# Unit tests
make test
# Test rendering
make render
```
The binaries will be generated in `build/` directory.

Generate documentation with `doxygen` (you need to have doxygen installed in your system):
```bash
doxygen doxygen.conf
```
The index will be located in `docs/html/index.html`.
