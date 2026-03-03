# Building the engine

Brenta-Engine is written C++23 and uses `cmake` as it's build
system. You can compile the engine as a static or shared library for
both Windows and Linux.

## Dependencies

In order to build the engine, you will need:

- A compiler for `C++23`

- `cmake` >= 3.16

The rest of the dependencies, like assimp and glfw, are present in the
source tree as git submodules under the `external/` directory. These
will be automatically built by the build system.

## Clone the repo

Clone the project from GitHub, use `--recurse-submodules` to pull the
dependencies.

```bash
git clone --recurse-submodules -j8 https://github.com/San7o/Brenta-Engine.git
cd Brenta-Engine
```

If you already cloned the repo without `--recurse-submodules`, you 
can still pull them with:

```bash
git submodule update --init --recursive
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
cmake -Bbuild -D BRENTA_BUILD_EXAMPLES=on
cmake --build build -j$(nproc) --target <example-name>
```

where `example-name` is the name of the `.cpp` example file without
extension. You can also omit the `--target` option completely, this
will build all examples.

To build tests:

```bash
cmake -Bbuild -D BRENTA_BUILD_TESTS=on
cmake --build build -j$(nproc)
./buid/tests --no-multithread
```

By default, the engine in compiled with support for
[ImGUI](https://github.com/ocornut/imgui) and
[ECS](https://github.com/San7o/viotecs) .  If you want to disable this
(for example, if you want a smaller build and faster compile time) you
can pass `BRENTA_USE_IMGUI=off` and `BRENTA_USE_ECS=off` to cmake:

```bash
cmake -Bbuild -D BRENTA_USE_ECS=off
```

After compilation, all binaries will be generated in `build/`
directory.

## Building website documentation

To build the wensite and documentation you need to have
[pandoc](https://pandoc.org/) and
[doxygen](https://www.doxygen.nl/index.html) installed. Then, build
with:

```bash
make html
```

The index will be located in `docs / html / index.html`.

## Generating shaders

To generate shaders bindings for C/C++, run the following command:

```bash
make shaders
```
