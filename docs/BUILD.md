# Building the engine

The engine is written C++ and uses `cmake` as it's build system. The
engine can be compiled as a static or shared library. This file
contains a guide for all the steps to succesfully build the engine
library, examples and tests in your machine.

## Supported Platforms

Currently the only officially supported platform is Linux. The engine
may work on Windows or MacOS but it is not officially supported at the
moment. If you want to make sure that it builds succesfully on other
platforms, consider [CONTRIBUTING.md](./CONTRIBUTING.md) to this
project. This guide will go through the build process on a Linux
system.

## Dependencies

In order to build the engine, you will need:

- A `C++23` compiler

- `cmake` >= 3.16

- `SDL3` for audio, make sure it is compiled for your audio driver (it
  is already configured correctly if you are using the library from
  your distribution). This dependency will be dropped in favour of
  [miniaudio](https://github.com/mackron/miniaudio).

The rest of the dependencies, like assimp and glfw, are present in the
source tree as git submodules under the `external/` directory. These
will be automatically built by the build system if you want a static
build.

## Clone the repo

Clone the project from GitHub with `--recurse-submodules` to download
the dependencies.

```bash
git clone --recurse-submodules -j8 https://github.com/San7o/Brenta-Engine.git
cd Brenta-Engine
```

## Build

Building is done through `cmake`. To compile the dynamic library,
simply run:

```bash
cmake -Bbuild
cmake --build build -j$(nproc)
```

For the static library:

```bash
cmake -Bbuild -DBUILD_SHARED_LIBS=off
cmake --build build -j$(nproc)
```

To compile the examples:

```bash
cmake -Bbuild -DBRENTA_BUILD_EXAMPLES=on
cmake --build build -j$(nproc) --target <example-name>
```

where `example-name` is the name of the `.cpp` example file without
extension.

To build tests:

```bash
cmake -Bbuild -DBRENTA_BUILD_TESTS=on
cmake --build build -j$(nproc)
./buid/tests --no-multithread
```

Additionally, you can decide to compile with support for
[ImGUI](https://github.com/ocornut/imgui) with `BRENTA_USE_IMGUI` and
for the [ECS](https://github.com/San7o/viotecs) library with
`BRENTA_USE_ECS`.

All binaries will be generated in `build/` directory.

## Building website documentation

You can build the website and `doxigen` documentation with:

```bash
make html
```

The index will be located in `docs/html/index.html`.

# Format code

To format the code using `clang-format`:

```bash
make format
```
