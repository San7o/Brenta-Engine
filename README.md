# opengl-santo-engine

A straightforward and simple Entity Component System (ECS) engine, developed in C++
and utilizing OpenGL for advanced graphics rendering. This engine is designed to
provide a modular and flexible architecture for game development and simulations.

## 🕵️‍♂️ How it works

Create a `component`:

```c++
struct ModelComponent : Types::Component {
    Model model;
    Types::ShaderName shader;

    /* I added a constructor for ease of use */
    ModelComponent(Model model, Types::ShaderName shader)
            : model(model), shader(shader) {}
};
```

Create a `system`:

```c++
auto renderer = std::make_shared<System>("Renderer", []() {
    /* Get the entities with the model component */
    auto matches = World::QueryComponents({"ModelComponent"});
    if (matches.empty()) return;

    for (auto match : matches) {
        /* Get the model component */
        auto model_component = static_cast<ModelComponent*>
             (World::EntityToComponent(match, "ModelComponent"));
        
        /* Translate the model */
        // ...
        
        myModel.Draw(default_shader);
    }
});
World::AddSystem(renderer);
```

Create an `entity`:

```c++
/* Create the player entity */
auto player_entity = World::NewEntity();


/* Add the player component */
auto player_component = std::make_shared<PlayerComponent>();
World::AddComponent(player_entity, "PlayerComponent", player_component);


/* Load the shader */
Shader::NewShader("default_shader",
                  std::filesystem::absolute("game/shaders/shader.vs"),
                  std::filesystem::absolute("game/shaders/shader.fs"));


/* Load the model */
Model model(std::filesystem::absolute("assets/models/backpack/backpack.obj"));


/* Add the model component */
auto model_component = std::make_shared<ModelComponent>(model, "default_shader");
World::AddComponent(player_entity, "ModelComponent", model_component);

```

Example `main.cpp`:

```c++
int main() {

    /* Set Log level, default = WARNING */
    Logger::SetLogLevel(Types::LogLevel::INFO);

    /* Initialize the screen */
    Screen::Init(SCR_WIDTH, SCR_HEIGHT);

    /* Load OpenGL */
    GL::LoadOpenGL((GLADloadproc)Screen::GetProcAddress(),
                    SCR_WIDTH, SCR_HEIGHT);

    /* Initialize the world */
    World::Init();
    InitPlayer();
    InitRenderer();
    // ...

    while(!Screen::isWindowClosed()) {

        /* Input */
        if (Screen::isKeyPressed(GLFW_KEY_ESCAPE))
            Screen::SetClose();

        /* Clear */
        GL::SetColor(0.2f, 0.3f, 0.3f, 1.0f);
        GL::Clear();

        World::Tick();

        Screen::PollEvents();
        Screen::SwapBuffers();
    }
    
    World::Delete();
    Screen::Terminate();
    return 0;
}
```

There are many other examples in the `examples` directory.

Here is an high lievel simplified view of the system:

![image](https://github.com/user-attachments/assets/d76b238d-56f1-4b57-8140-400af6ed1d23)

## 🦞 Current State

The project is structured into the 3 folders `engine`, `game`, `src` 
containing respectively the game engine, a game demo and a render demo.

The following has been implemented on the engine:

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

- [x] Logging module

Screenshots and videos:

https://github.com/user-attachments/assets/8430fb69-66bb-4457-bdce-a87506b78235

![image](https://github.com/user-attachments/assets/955611fb-3eeb-45a2-adc0-2a0b55680de1)

## 🔨 Todo

- [ ] Load Opengl should take no arguments

Default ECS:

- [ ] Game state resource

- [ ] Transform component

- [ ] Model component


# 💀 Dependencies

You need

- A `C++17` compiler

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

# 🪖 Compile

You can compile the demo game with `cmake`:
```bash
cmake -Bbuild
cmake --build build
./build/main
```

Or with `make`:
```bash
# Demo game using the engine
make game
# Unit tests
make test
# Test rendering
make render
```

The binaries will be generated in `build/` directory.

## 👴 Future

A GUI for the engine would be interesting to implement in the long future.
