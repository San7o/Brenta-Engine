# opengl-santo-engine


https://github.com/user-attachments/assets/8430fb69-66bb-4457-bdce-a87506b78235

![image](https://github.com/user-attachments/assets/955611fb-3eeb-45a2-adc0-2a0b55680de1)


This project aims to build a simple OpenGL application that
includes:

- [x] triangles

- [x] textures

- [x] camera

- [x] lighting

- [x] model loading

- [ ] Optional: advanced features

## Vision

I want to build a functional ECS system for the engine, and run physics symulations.

# Dependencies

You need

- A `C/C++` compiler

- `GLFW` isntalled on your machine

If you are on `NixOS`, you can access the developement environment
by running:
```bash
nix shell
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

## Compile

You can compile the source with `make`:
```bash
make
```
Run with
```bash
make run
```

The binaries will be generated in `build/` directory.

## TODO
- [ ] Logging system
- [ ] ECS style system
