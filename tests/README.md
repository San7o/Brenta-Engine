# Tests

The engine uses It's own custom testing library [valFuzz](https://github.com/San7o/valFuzz)
which should already be downloaded and properly configured In the engine's cmake file.

To build and run test, use the following commands:
```bash
cmake -Bbuild -DBUILD_TESTS=ON
cmake --build build -j 4
./buid/tests --no-multithread
```
Notice the `-no-multithread`, since much of the engine uses global data, having multiple
threads for the tests would lean to a bad experience.
