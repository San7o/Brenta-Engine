# Tests

The engine uses it's own custom testing library
[valFuzz](https://github.com/San7o/valFuzz). To build and run the
tests, use the following commands:

```bash
cmake -Bbuild -DBRENTA_BUILD_TESTS=ON
cmake --build build -j 4
./buid/tests --no-multithread
```

Notice the `-no-multithread`, since much of the engine uses some
global state, having multiple threads for the tests would be a bad
idea.
