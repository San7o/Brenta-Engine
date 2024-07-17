# opengl-santo-engine
This project aims to build a simple ECS (Entity Component System) engine focused on physics simulations using OpenGL in C++.

Here is an example of how to use the engine:
```c++
World::Init();
std::cout << "Welcome to my Game!" << std::endl;

/* New entity */
Entity entity = World::NewEntity();

/* Add a health component to the entity */
struct Health : Component {
    int value;
    Health(Entity entity, int value)
            : Component(entity, "Health"), value(value) {}
};
auto health = std::make_shared<Health>(entity, 100);
World::AddComponent(health);

/* System to decrease health each tick */
auto poison = std::make_shared<System>("Poison", []() {
    auto h = static_cast<Health*>
                      (World::getComponents()->at("Health")
                       .at(0).get());
    h->value--;
    std::cout << "Health: " << h->value << std::endl;
});
World::AddSystem(poison);

/* Main loop */
for(int i = 0; i < 10; i++) {
    World::Tick();
}

World::Delete();
return 0;
```

And here is an high, simplified, level view of the system:

![image](https://github.com/user-attachments/assets/d76b238d-56f1-4b57-8140-400af6ed1d23)


The project is structured into the 3 folders `engine`, `game`, `engine-gui` containing the
following classes:

![image](https://github.com/user-attachments/assets/f825bdc2-9345-49ef-a87d-90939ba47e07)

## Current State

Currently, I implemented the following features:
- [x] triangles

- [x] textures

- [x] camera

- [x] lighting
  - Ambient
  - Diffuse
  - Specular   

- [x] model loading

- [x] Other OpenGL features
  - Texture transparency
  - Face culling
  - MSAA

- [x] ECS

Screenshots and videos:

https://github.com/user-attachments/assets/8430fb69-66bb-4457-bdce-a87506b78235

![image](https://github.com/user-attachments/assets/955611fb-3eeb-45a2-adc0-2a0b55680de1)

## Todo

- [ ] Query components

- [ ] Game state

- [ ] Make all paths full

- [ ] Error checks with `GLenum glGetError()`

- [ ] Unit tests

- [ ] Logging system


# Dependencies

You need

- A `C/C++` compiler

- `GLFW` installed on your machine

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

You can compile the source with `cmake`:
```bash
cmake -Bbuild
cmake --build build
./build/main
```

Or with `make`:
```bash
make game
```

Run tests with:
```bash
make test
```

The binaries will be generated in `build/` directory.
