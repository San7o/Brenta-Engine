# Guide

Welcome to this quick guide on most of the features of the engine. For
each feature, an example with context will be shown. Enjoy the read :)

For updated examples, see [examples](../examples/README.md).

## Engine Class

To initialize any subsystem, you can use the `Engine` class. This is
not mandatory, you can initialize and destroy the subsystems manually,
but the Engine class provides a nice way to do it.

To create an engine, you can use the Builder class:

```cpp
#include <brenta/brenta.hpp>

int main() {
  Engine::Builder()
    .with(Logger::Builder()
          .level(oak::level::debug))
    .with(Window::Builder()
          .title("load opengl test")
          .width(screen_width)
          .height(screen_height))
    .build();
  auto engine = Engine::managed();

  while (!Window::should_close())
  {
    // Handle input...
    if (Window::is_key_pressed(Key::Escape))
      Window::close();
    
    // Update logic...
    // Draw...
    Gl::set_color(Color::grey());
    Gl::clear();

    Window::poll_events();
    Window::swap_buffers();
  }
  return 0;
}
```

We will now go through some subsystems and explain how to use them.

## Input

You can register callback functions for the input. Those functions are
called when the specified `key` is pressed, or the mouse is moved,
depending on the callback you register.

```cpp
auto toggle_wireframe_callback = []() {

    auto wireframe = world::get_resource("WireframeResource");
    if (wireframe == nullptr) return;

    gl::set_poligon_mode(!wireframe->enabled);
    wireframe->enabled = !wireframe->enabled;
};

Input::add_keyboard_callback(Key::F, toggle_wireframe_callback);
```

In this example we register a keyboard callback that toggles the
wireframe mode when the `F` key is pressed. You can use
`Brenta::Input::AddMousePosCallback` to register a mouse callback,
this ill be called with the x and y position of the mouse.

You can also remove the callbacks with
`brenta::input::remove_keyboard_callback` and
`brenta::input::remove_mouse_pos_callback`.

## Audio

The audio subsystem is very simple: there are audio streams and audio
files, you can play an audio file on a stream (not multiple streams)
and stop it, so you need to have multiple streams if you want to play
multiple audio files at the same time.

You can load an audio file like so:

```cpp
Audio::load("guitar", "assets/audio/guitar.wav");
```

We are identifying this audio file with the name `guitar`.

You can create a stream with the name "music" like so:

```cpp
Audio::create_stream("music");
```

And finally play the `guitar` audio like so:

```cpp
Audio::play("guitar");
```

You can Pause and Resume streams with `brenta::audio::stream_pause` and
`brenta::audio::stream_resume`, set the volume and stop it. You can find
the API in `Brenta::Audio`.

## Particles

You can create and customize particles via the
`brenta::particle_emitter` class. All the computation is done in the
GPU so the engine can handle lots and lots of particles. Here's a
quick look on the API:

```cpp
auto emitter = ParticleEmitter::Builder()
       .starting_position(glm::vec3(0.0f, 0.0f, 5.0f))
       .starting_velocity(glm::vec3(0.0f, 5.0f, 0.0f))
       .starting_spread(glm::vec3(10.0f, 10.0f, 10.0f))
       .starting_time_to_live(0.5f)
       .num_particles(1000)
       .spawn_rate(0.01f)
       .scale(1.0f)
       .atlas_path"assets/textures/particle_atlas.png")
       .atlas_width(8)
       .atlas_height(8)
       .atlas_index(45)
       .build();

// Inside the game loop:
emitter.update(Window::get_time().get_delta());
emitter.render();
```

## Logger

Check out [oak](https://github.com/San7o/oak)! The engine uses oak as the
logger, you can set the log level and the log file in the engine builder.
You can log messages like so:
```cpp
oak::info("Hello, world!");
```
Oak has many more advanced features, I suggest you check out the repository.

## Text

The `brenta::text` subsystem allows you to render text on the screen. You can
set the font and font size of your text, and render it in the main loop like
this:
```cpp
Text::render_text("Hello OpenGL!", x, y, scale, glm::vec3(r, g, b));
```

## ECS

Everything in the ECS exists in the `World`, you can think of it as a
global state of everything that is happening.  The World contains
`Entities`, those are the most elemental things that exist.  You can
add `Components` to entities, which are their properties (like Health,
Position, Mesh). You interact with those components through `Systems`
by making `Queries` on their components. There are also `Resources`
that store global data. Uh that was quick, read it again if you need
it to.  Now we will go a bit deeper on how this works.

### The Main loop

The main loop calls `world::tick()`. At each tick, all the Systems
will be called in the order they were added in the World.

The engine provides functions to interact with the window in
`Brenta::window`, some OpenGL helper functions in `brenta::gl`, a nice
`brenta::logger`, input handling with `brenta::input`, manage time
with `brenta::time`, display text with `brenta::text` and more!

```c++
#include <brenta/brenta.hpp>
#include <viotecs/viotecs.hpp>

using namespace brenta;
using namespace viotecs;

REGISTER_SYSTEMS(none);

int main()
{
  Engine::Builder()
    .with(Logger::Builder()
          .level(oak::level::debug)
          .file("/tmp/brenta-logs"))
    .with(Window::Builder()
          .title("brenta demo")
          .width(800)
          .height(600)
          .vsync()
          .msaa())
    .with(Input::Builder())
    .with(Ecs::Builder())
    .build();
  auto engine = Engine::managed();
  
  // Your init functions ...
  init_player();
  init_renderer();

  while(!Window::should_Close()) {

    Gl::set_color(Color::gray());
    Gl::clear();

    world::tick();

    Window::poll_wvents();
    Window::swap_buffers();
  }
  
  // The engine will take care of deallocation
  // of the submodules
  return 0;
}
```

### Component

A Component is a piece of data (more precisely, a struct) that gets
assigned to an Entity.

You can define your own component like so:

```c++
// This is a component
struct model_component : component {
  Model mod;
  Shader::Name shader;

  // You need to provide a default constructor
  model_component() {};

  // Any other construtor is optional
  model_component(model mod, Shader::Name shader)
        : model(model), shader(shader) {}
};
```

### System

A System is a function that gets called at each Tick in the reder
loop. It contains all the logic of the World. You will interact with
the Entities, Components and Resources via queries. You can specify an
entity to query by adding components to `system<...>`, the World will
provide you with an `std::vector<entity_t>` of the entities that have
all the components you specified.

Here is an example:

```c++
// Specify ModelComponent and TransformComponent query
struct renderer_system : system<model_component, transform_component> {

  // You need to define this function
  void run(std::vector<entity_t> matches) const override {
    if (matches.empty()) return;

    for (auto match : matches) {
      // Get the model component
      auto model_c = world::entity_to_component<model_component>(match);
      auto my_model = model_c->mod;

      // Translate the model
      // ...

      my_model.draw(default_shader);
    }
  }
};

// Somewhere in your code you need to
// have one (and only one) call on this macro
REGISTER_SYSTEMS(render_system);
```

### Entity

You can create Entities and assign Components to them like so:

```c++
// Create the player entity
auto player_entity = world::new_entity();


// Add the player component to the player entity
world::add_component<player_component>(player_entity, player_component());

// Load model and shader
// ...

// Add the model component to the player entity
auto model_c = model_component(mod, "default_shader");
world::add_component<model_component>(player_entity, model_c);
```

### Resources

Resources hold global data accessible via `world::get_resource<name>()`.
You can define a Resource like so:

```c++
// This is a resource
struct wireframe_resource : resource {
  bool enabled;
  wireframe_resource(bool e) : enabled(e) {}
};

world::add_resource<wireframe_resource>(wireframe_resource(false));
```

Here is an high lievel simplified view of those objects:

![image](https://github.com/user-attachments/assets/d76b238d-56f1-4b57-8140-400af6ed1d23)


There are many other examples in the `examples` directory and in the
`demo` which is guaranteed to be updated to the lastest APIs.

## Directory structure

- `CMakeLists.txt`: build system with cmake
- `docs/`: markdown documentation
  - `html/`: html website hosted with GH pages
- `examples/`: several example programs
- `external/`: dependencies
- `include`
  - `brenta/`: engine headers
- `LICENSE`
- `Makefile`: provides useful make commands
- `README.md`
- `src/`: engine Sources
- `tests/`: engine Tests
- `utils/`
  - `docs-images`: images
  - `doxygen`: doxigen configuration files
  - `website`: files for website generation
