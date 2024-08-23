# Building the engine

The engine is written entirely in C++ and uses `cmake` as It's build system. This file
contains a guide on all the steps to succesfully build the engine in your machine.

## Supported Platforms

Currently, the only officially supported platform is Linux. The developer is aiming
to release the engine on Windows and MacOS in the future. If you want to contribute
to this issue, feel free to do so and check out [CONTRIBUTING.md](./CONTRIBUTING.md)
for directions. This guide will go through the process on a linux system.

# Dependencies

In order to build the engine, you will need:

- A `C++17` compiler

- `GLFW`

- `libfreetype`

- `libassimp`

- `SDL3` for audio, make sure you compile for your audio driver

- `doxygen` for building documentation

You may or may not have the packages available in your packet manager. This
guide will cover how to compile the libraries for any linux distribution.

## Nix

If you are on the nix packet manager, a developement shell is provided for you
in [flake.nix](./flake.nix). To enter the environment, please run:
by running:
```bash
nix develop
```

## Building dependencies

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

# Compile

You can compile the demo game with `cmake` (recommended):
```bash
cmake -Bbuild
cmake --build build
./build/main
```
The binaries will be generated in `build/` directory.

## Building documentation

You can build the documentation with `doxygen` (you need to have doxygen installed in your system):
```bash
doxygen doxygen.conf
```
The index will be located in `docs/html/index.html`.

## Format code
To run the formatter, run:
```bash
make format
```

# Tests

To build tests, run:
```bash
cmake -Bbuild -DBUILD_TESTS=ON
cmake --build build -j 4
./buid/unit
```
