# Examples

This directory contains a examples that showcase how to use the
engine's api.

You can run examples with the following commands:

```bash
cmake -Bbuild -DBRENTA_BUILD_EXAMPLES=ON
cmake --build build -j 4 --target load_model
./build/load_model
```

The name of the target is the same as the source `.cpp` file. The
executable expects to be run from the root directory of the project
in order to resolve the asset files.
