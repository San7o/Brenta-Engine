# Examples

This directory contains a bunch of examples for you to learn the engine's
api as a reference other then the documentation.

You can run examples with the following commands:
```bash
cmake -Bbuild -DBUILD_EXAMPLES=ON
cmake --build build -j 4 --target load_model
./build/load_model
```

You can use the name of the files as targets to cmake.
