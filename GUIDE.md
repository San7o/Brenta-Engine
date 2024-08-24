# Guide

Welcome to this quick guide on most of the features of the engine. For each
feature, an example with context will be shown. Enjoy the read :)

For updated examples, see [examples](./examples/README.md).

## 🕵️‍♂️ How it works

In brief (takes a deep breath): everything in the ECS exists in the `World`,
you can think of it as a global state of everything that's happening.
The World contains `Entities`, those are the most elemental thing to exist.
You can add `Components` to entities, which are their "properties"
(like Health, Position, Mesh). You interact  with those components
through `Systems` by making `Queries` on their components. There are also `Resources`
that store global data. Uh that was quick, read it again if you need it to.
Now we will go a bit deeper on how this works, let's take a look:

### The Main loop

The main loop calls `World::Tick()`. At each tick, all the 
Systems will be called in the order they were added in 
the World. 

The engine provides functions to interact with the window in `Brenta::Screen`, 
some OpenGL helper functions in `Brenta::GL`, a nice `Brenta::Logger`,
input handling with `Brenta::Input`, manage time with `Brenta::Time`,
display text with `Brenta::Text` and more!

```c++
#include "engine.hpp"
#include "ecs.hpp"
using namespace Brenta;
using namespace Brenta::ECS;

int main() {

    Logger::Init();
    /* 
     * Set Log level, default = WARNING
     * The logger will automatically save
     * all logs in "./logs/log.txt". Check out
     * "common/engine_logger.h" for more
     * functionalities
     */
    Logger::SetLogLevel(Types::LogLevel::INFO);

    /* Initialize the screen */
    Screen::Init(SCR_WIDTH, SCR_HEIGHT);

    /* Load OpenGL */
    GL::LoadOpenGL();

    /* Initialize the world */
    World::Init();

    InitPlayer();
    InitRenderer();
    // Your init functions ...

    /* 
     * It's nice to reset the time
     * before starting the game loop
     * so that the first frame will have
     * a delta time of 0.
     */
    Time::Update(Screen::GetTime());
    while(!Screen::isWindowClosed()) {

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

### Component

A Component is a piece of data (more precisely, a struct) that
gets assigned to an Entity. 

You can define your own component like so:

```c++
/* This is a component */
struct ModelComponent : Component {
    Model model;
    Types::ShaderName shader;

    /* You need to provide a default constructor */
    ModelComponent() {};

    /* Any other construtor is optional */
    ModelComponent(Model model, Types::ShaderName shader)
            : model(model), shader(shader) {}
};
```

### System

A System is a function that gets called at each 
Tick in the reder loop. It contains all the logic of 
the World. You will interact with the Entities, 
Components and Resources via queries. You can specify
an entity to query by adding components to System<...>,
the World will provide you with an std::vector<Entity>
of the entities that have all the components you
specified.

Here is an example:

```c++
/* Specify ModelComponent and TransformComponent query */
struct RendererSystem : System<ModelComponent, TransformComponent> {

    /* You need to define this function */
    void run(std::vector<Entity> matches) const override {
      if (matches.empty()) return;

      for (auto match : matches) {
          /* Get the model component */
          auto model_component = World::EntityToComponent<ModelComponent>(match);
          
          /* Translate the model */
          // ...

          myModel.Draw(default_shader);
      }
    }
};

/* 
 * Somewhere in your code you need to
 * have one (and only one) call on this macro
 */
REGISTER_SYSTEMS(RenderSystem);

```

### Entity

You can create Entities and assign Components to them like so:

```c++
/* Create the player entity */
auto player_entity = World::NewEntity();


/* Add the player component to the player entity */
World::AddComponent<PlayerComponent>(player_entity, PlayerComponent());

/* Load model and shader */
// ...

/* Add the model component to the player entity */
auto model_component = ModelComponent(model, "default_shader");
World::AddComponent<ModelComponent>(player_entity, model_component);

```

### Resources

Resources hold global data accessible via `World::GetResource<Name>()`.
You can define a Resource like so:

```c++
/* This is a resource */
struct WireframeResource : Resource {
    bool enabled;
    WireframeResource(bool e) : enabled(e) {}
};

World::AddResource<WireframeResource>(WireframeResource(false));
```

### Callbacks

Callbacks are funcitons that are called when the specified `key` is pressed,
the code responsible for this is in `ECS::Input`. Here is an example:
```c++
auto toggle_wireframe_callback = []() {

    auto wireframe = World::GetResource("WireframeResource");
    if (wireframe == nullptr) return;

    GL::SetPoligonMode(!wireframe->enabled);
    wireframe->enabled = !wireframe->enabled;
};

Input::AddCallback(GLFW_KEY_F, toggle_wireframe_callback);
```

### Particles

You can create and customize particles via the `Brenta::ParticleEmitter` class. All
the computation is done in the GPU so the engine can handle lots and lots
of particles. Here's a quick look on the API:

```C++
/* Nice builder patterns */
ParticleEmitter emitter = ParticleEmitter::Builder()
        .set_starting_position(glm::vec3(0.0f, 0.0f, 5.0f))
        .set_starting_velocity(glm::vec3(0.0f, 5.0f, 0.0f))
        .set_starting_spread(glm::vec3(10.0f, 10.0f, 10.0f))
        .set_starting_timeToLive(0.5f)
        .set_num_particles(1000)
        .set_spawn_rate(0.01f)
        .set_scale(1.0f)
        .set_atlas_path(std::filesystem::absolute(
            "assets/textures/particle_atlas.png"
        ).string())
        .set_atlas_width(8)
        .set_atlas_height(8)
        .set_atlas_index(45)
        .build();

// Inside the game loop:
emitter.updateParticles(Time::GetDeltaTime());
emitter.renderParticles();
```


### Audio

There is a simple-to-use audio API in `Brenta::Audio`. You can load sound files,
create channels and play a sound on a channel. There can be only one sound
playing per channel, but of course there can be multiple channels playing
so some channel management from the developer is needed:

```c++
Audio::LoadAudio("guitar", std::filesystem::absolute("assets/audio/guitar.wav"));
Audio::PlayAudio("guitar"); /* This will use the "default_stream", or you can specify
                               a particular stream */
Audio::CreateStream("background_music");
Audio::PlayAudio("guitar", "background_music");
```

There are many other examples in the `examples` directory and in the `game`
which is guaranteed to be updated to the lastest APIs.

Here is an high lievel simplified view of those objects:

![image](https://github.com/user-attachments/assets/d76b238d-56f1-4b57-8140-400af6ed1d23)
