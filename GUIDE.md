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

The main loop calls `world::tick()`. At each tick, all the 
Systems will be called in the order they were added in 
the World. 

The engine provides functions to interact with the window in `Brenta::Screen`, 
some OpenGL helper functions in `brenta::gl`, a nice `brenta::logger`,
input handling with `brenta::input`, manage time with `brenta::time`,
display text with `brenta::text` and more!

```c++
#include "engine.hpp"
#include "ecs.hpp"
using namespace brenta;
using namespace brenta::ecs;

// Initialize a default camera
namespace brenta
{
    camera default_camera = camera();
}

int main() {

    engine eng = engine::builder()
        .use_screen(true)
        .use_audio(true)
        .use_input(true)
        .use_logger(true)
        .use_text(true)
        .use_ecs(true)
        .set_screen_width(SCR_WIDTH)
        .set_screen_height(SCR_HEIGHT)
        .set_screen_is_mouse_captured(false)
        // ...
        .build();

    // Your init functions ...
    init_player();
    init_renderer();

    /* 
     * It's nice to reset the time
     * before starting the game loop
     * so that the first frame will have
     * a delta time of 0.
     */
    time::update(screen::get_time());
    while(!screen::is_window_closed()) {

        gl::set_color(0.2f, 0.3f, 0.3f, 1.0f);
        gl::clear();

        world::tick();

        screen::poll_wvents();
        screen::swap_buffers();
    }
    
    // The engine will take care of deallocation
    // of the submodules
    return 0;
}
```

### Component

A Component is a piece of data (more precisely, a struct) that
gets assigned to an Entity. 

You can define your own component like so:

```c++
/* This is a component */
struct model_component : component {
    model mod;
    types::shader_name_t shader;

    /* You need to provide a default constructor */
    model_component() {};

    /* Any other construtor is optional */
    model_component(model mod, types::shader_name_t shader)
            : model(model), shader(shader) {}
};
```

### System

A System is a function that gets called at each 
Tick in the reder loop. It contains all the logic of 
the World. You will interact with the Entities, 
Components and Resources via queries. You can specify
an entity to query by adding components to `system<...>`,
the World will provide you with an `std::vector<entity_t>`
of the entities that have all the components you
specified.

Here is an example:

```c++
/* Specify ModelComponent and TransformComponent query */
struct renderer_system : system<model_component, transform_component> {

    /* You need to define this function */
    void run(std::vector<entity_t> matches) const override {
      if (matches.empty()) return;

      for (auto match : matches) {
          /* Get the model component */
          auto model_c = world::entity_to_component<model_component>(match);
          auto my_model = model_c->mod;

          /* Translate the model */
          // ...

          my_model.draw(default_shader);
      }
    }
};

/* 
 * Somewhere in your code you need to
 * have one (and only one) call on this macro
 */
REGISTER_SYSTEMS(render_system);
```

### Entity

You can create Entities and assign Components to them like so:

```c++
/* Create the player entity */
auto player_entity = world::new_entity();


/* Add the player component to the player entity */
world::add_component<player_component>(player_entity, player_component());

/* Load model and shader */
// ...

/* Add the model component to the player entity */
auto model_c = model_component(mod, "default_shader");
world::add_component<model_component>(player_entity, model_c);
```

### Resources

Resources hold global data accessible via `world::get_resource<name>()`.
You can define a Resource like so:

```c++
/* This is a resource */
struct wireframe_resource : resource {
    bool enabled;
    wireframe_resource(bool e) : enabled(e) {}
};

world::add_resource<wireframe_resource>(wireframe_resource(false));
```

### Callbacks

Callbacks are funcitons that are called when the specified `key` is pressed,
the code responsible for this is in `ecs::input`. Here is an example:
```c++
auto toggle_wireframe_callback = []() {

    auto wireframe = world::get_resource("WireframeResource");
    if (wireframe == nullptr) return;

    gl::set_poligon_mode(!wireframe->enabled);
    wireframe->enabled = !wireframe->enabled;
};

input::add_callback(GLFW_KEY_F, toggle_wireframe_callback);
```

### Particles

You can create and customize particles via the `Brenta::ParticleEmitter` class. All
the computation is done in the GPU so the engine can handle lots and lots
of particles. Here's a quick look on the API:

```C++
/* Nice builder patterns */
particle_emitter emitter = particle_emitter::builder()
        .set_starting_position(glm::vec3(0.0f, 0.0f, 5.0f))
        .set_starting_velocity(glm::vec3(0.0f, 5.0f, 0.0f))
        .set_starting_spread(glm::vec3(10.0f, 10.0f, 10.0f))
        .set_starting_time_to_live(0.5f)
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
emitter.update_particles(time::get_delta_time());
emitter.render_particles();
```


### Audio

There is a simple-to-use audio API in `brenta::audio`. You can load sound files,
create channels and play a sound on a channel. There can be only one sound
playing per channel, but of course there can be multiple channels playing
so some channel management from the developer is needed:

```c++
audio::load_audio("guitar", std::filesystem::absolute("assets/audio/guitar.wav"));
audio::play_audio("guitar"); /* This will use the "default_stream", or you can specify
                               a particular stream */
audio::create_stream("background_music");
audio::play_audio("guitar", "background_music");
```

There are many other examples in the `examples` directory and in the `game`
which is guaranteed to be updated to the lastest APIs. Check out the full documentation
for a deeper look.

Here is an high lievel simplified view of those objects:

![image](https://github.com/user-attachments/assets/d76b238d-56f1-4b57-8140-400af6ed1d23)
