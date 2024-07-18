# opengl-santo-engine
This project aims to build a simple ECS (Entity Component System) engine focused on physics simulations using OpenGL in C++.

Here is an example of how to use the engine:
```c++
World::Init();
std::cout << "Welcome to my Game!" << std::endl;

/* New entity as the player */
Entity player_entity = World::NewEntity();

/* Add the Player component to the entity */
struct Player : Component {
    Player(Entity entity)
            : Component(entity, "Player") {}
};
auto playerComponent = std::make_shared<Player>(player_entity);
World::AddComponent(playerComponent);

/* Add a health component to the entity */
struct HealthComponent : Component {
    int value;
    HealthComponent(Entity entity, int value)
            : Component(entity, "Health"), value(value) {}
};
auto health_component = std::make_shared<HealthComponent>(player_entity, 100);
World::AddComponent(health_component);

/* System to decrease health to the player */
auto poison = std::make_shared<System>("Poison", []() {
    /* Query entities with both PlayerComponent and HealthComponent */
    auto matches = World::QueryComponents({"Player", "Health"});
    if (matches.empty()) {
        return;
    }

    /* Get health component */
    auto health = static_cast<HealthComponent*>
                 (World::EntityToComponent(matches.at(0), "Health"));

    health->value--;
    std::cout << "Health: " << health->value << std::endl;
});
World::AddSystem(poison);

/* Create a global resource */
struct GlobalResource : Resource {
    int value;
    GlobalResource(int value) :
            Resource("GlobalResource"), value(value) {}
};
World::AddResource(std::make_shared<GlobalResource>(10));

/* Main loop */
for(int i = 0; i < 10; i++) {
    World::Tick();
}

World::Delete();
```

And here is an high, simplified, level view of the system:

![image](https://github.com/user-attachments/assets/d76b238d-56f1-4b57-8140-400af6ed1d23)


The project is structured into the 3 folders `engine`, `game`, `engine-gui` containing the
following classes:

![image](https://github.com/user-attachments/assets/f825bdc2-9345-49ef-a87d-90939ba47e07)

## Current State

Currently, I implemented the following features:
- [x] Triangles

- [x] Textures

- [x] Camera

- [x] Shaders

- [x] Lighting
  - Ambient
  - Diffuse
  - Specular   

- [x] Model loading

- [x] Other OpenGL features
  - Texture transparency
  - Face culling
  - MSAA

- [x] ECS
  - Entities
  - Components
  - Systems
  - World
  - Query multiple components

- [x] Unit tests

Screenshots and videos:

https://github.com/user-attachments/assets/8430fb69-66bb-4457-bdce-a87506b78235

![image](https://github.com/user-attachments/assets/955611fb-3eeb-45a2-adc0-2a0b55680de1)

## Todo

- [ ] Game state

- [ ] Error checks with `GLenum glGetError()`

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
