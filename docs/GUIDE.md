# Guide

Welcome to the Brenta-Engine guide. This document explains how the
engine works and how to use its API for your own projects. There are
plenty of features and abstractions which may seem scary at first, but
hopefully you will conclude that these abstractions make sense and fit
elegantly together. The API is designed to be clear to use at any
abstraction level, and it makes it easy to combine objects together
without too much effort.

For an higher level overview, please read the [DESIGN](./DESIGN.md)
document first as it provides a good introduciton on the architecture
of the engine.

For updated examples see
[examples](https://github.com/San7o/Brenta-Engine/tree/main/examples),
this documentation may be slightly behind the latest API while the
examples are guaranteed to compile.

## API Introduction

All functions of the core brenta engine are under the `brenta`
namespace. Brenta also uses some external libraries that have their
own namespace such as `viotecs` for the ECS, `oak` for the logger,
`tenno` for the standard library and `valfuzz` for testing. These
libraries were once part of the engine's source code. They were
separated into their own projects to develop them independently from
the engine since they are really useful for other projects. Brenta
also uses external dependencies such as `ImGui` for GUI, you can
integrate their API directly with the rest of brenta functionalities.

### Builder pattern

The most frequent pattern you will encounter in the engine is the
`builder` patter. Almost every object can be constructed through their
builder, and its usage is encouraged throughout the entire API.

Here is an example:

```cpp
auto camera_builder =
    Camera::Builder()
    .projection_type(Camera::ProjectionType::Perspective)
    .position(Camera::Spherical::Builder()
              .center({0.0f, 2.0f, 0.0f})
              .phi(1.25f)
              .theta(1.25f)
              .radius(30.0f)
              .build())
    .fov(45.0f);
    
auto camera = camera_builder.build();
```

We used `Camera::Builder()` to default-initialize the builder object,
then we set relevant fields (usually all of them are optional) and
then call the `build()` method to construct the `Camera` object.

This is particularly nice when you have a lot of variables, for
example let's look at the `ParticleEmitter` builder:

```cpp
auto emitter = ParticleEmitter::Builder()
  .with_camera(camera)
  .starting_position(glm::vec3(0.0f, 0.0f, 0.0f))
  .starting_velocity(glm::vec3(0.0f, 5.0f, 0.0f))
  .starting_spread(glm::vec3(3.0f, 10.0f, 3.0f))
  .starting_time_to_live(0.5f)
  .num_particles(1000)
  .spawn_rate(0.99f)
  .scale(1.0f)
  .atlas_path("examples/assets/textures/particle_atlas.png")
  .atlas_width(8)
  .atlas_height(8)
  .atlas_index(3)
  .build();
```

As you can see, the pattern is similar to the `Camera` builder: we
first called `ParticleEmitter::Builder()`, then set the relevant
variabled and called `.build()`. We may also create the
`ParticleEmitter` object like this:

```cpp
auto emitter = ParticleEmitter::Builder()
  .with_camera(camera)
  .build();
```

This is completely valid, and all the variables we did not specify
will be set with default values. Another reason why builders are
awesome is that they are effectively "recipes" for constructing other
objects. If I wanted to generate 10 different particle emitters with
the same settings, I can reuse the same builder and call `.build()`
multiple times. This is used in the `AssetManager` to reload an object
when a change is detected (if hot reloading is activated): the builder
contains information about which files to listen to in the filesystem,
and when one of these files gets updated, the asset manager will build
a new object and release the old one.

### Passing objects

Since some objects own resources such as GPU buffers, these objects
are usually passed around the API either via:

- a builder
- moving the object
- through a shared or weak pointer


You will see that most of the APIs and constructors accept any or all
of these methods. For example, here are the constructors of a 
`ModelNodeComponent`:

```cpp

ModelNodeComponent() = default;
ModelNodeComponent(Model::Builder& m, bool transparent = false);
ModelNodeComponent(Model&& m, bool transparent = false);
ModelNodeComponent(tenno::shared_ptr<Model> m, bool transparent = false);
```

In general, more "high-level" objects (such as the scene-graph) work
with shared pointers, while "low-level" objects such as textures and
meshes store their data directly without pointers. If you only work
with these objects you are not required to create shared pointers for
them.

### Sybsystems and Engine

Another architectural concept you will see are `Sybsystems`. These are
static classes that have to be initialized and terminated to be used
correctly, and they provide a standard api to do so. Since they are
static, they don't use RAII to automatically manage their lifetime. To
avoid messing with many classes and lifetimes, all subsystems can be
managed by the `Engine` class.

Here is how it looks like:

```cpp
Engine::Builder()
  .with(Logger::Builder()
        .level(Logger::Level::debug))
  .with(Window::Builder()
        .title("load opengl test")
        .width(screen_width)
        .height(screen_height))
  .build();
auto engine = Engine::managed();
```

Here we used the `Engine::Builder()` to initialize a global `Logger`
and a `Window`. These are indeed subsystems. You can work with any
class that implements the `Subsystem` interface by adding it to the
`Engine` using `with(MyClass::Builder)`.

We then call `Engine::managed()` which will return an object (an
"engine manager") that will terminate the engine and all its
subsystems when it goes out of scope. If you don't want to use this,
you can initialize and terminate the engine manually:

```cpp
auto engine = Engine::instance();
engine.initialize();

// ...

auto engine = Engine::instance();
engine.terminate();
```

Each subsystem can be configured through their builder, as we have
already discussed. Like all other things, these usage of `Engine` is
not mandatory, but it provides a nice way to manage subsystems.

### Drivers

Some functionalities can be implemented with different backends. For
example, playing audio can be done with several libraries such as
[miniaudio](https://miniaud.io/), [SDL](https://www.libsdl.org/),
[PortAudio](https://www.portaudio.com/) and many others (sorry for not
including your favourite in the list). Some functionalities may even
be OS-dependent such as waiting for files in the filesystem (inotify
on Linux, iocp in Windows). To support multiple implementations
("backends") of the same API, the engine often uses a `Driver`
abstraction. A driver specifies an interface that can be implemented
by different classes.

In practice, it looks like the following:

```cpp
class AudioDriver
{
public:

  virtual void play(const SongId& id) = 0;
};

class SDLAudioDriver : public AudioDriver
{
public:
   
   void play(const SongId& id) override;

};
```

## Window

To draw anything on the screen you first need a window. You can manage
the window with the `Window` subsystem, here is a full program that
compiles and opens a window:

```cpp
#include <brenta/engine.hpp>
#include <brenta/window.hpp>

using namespace brenta;

int main()
{
  Engine::Builder()
    .with(Window::Builder()
          .title("simple screen")
          .width(800)
          .height(600))
    .build();
  auto engine = Engine::managed();
  
  while (!Window::should_close())
  {
    if (Window::is_key_pressed(Key::Escape))
      Window::close();

    Window::poll_events();
    Window::swap_buffers();
  }

  return 0;
}
```

You can use `Window` to get and update information about its width and
height, keeping track of time and closing it. Internally, the window
uses a `WindowDriver` to implement its functionalities. This makes
it possible to support multiple backends such as glfw or SDL.

## Input

Input handling can be done through callbacks using the `Input`
subsystem.

Callback are called when the specified `key` is pressed, or when the
mouse is moved, depending on the callback you register.

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
`Input::AddMousePosCallback` to register a mouse callback,
this ill be called with the x and y position of the mouse.

```cpp
Input::add_mouse_callback("rotate_camera",
                            [&](double x, double y)
{
  // ...
}
```

You can also remove the callbacks with
`Input::remove_keyboard_callback` and
`Input::remove_mouse_pos_callback`.

To keep track of the state of the mouse, you can use the handy `Mouse`
class.

```cpp
Mouse mouse = {};
mouse.set_sensitivity(0.05f);
mouse.set_x(x);
mouse.set_y(y);
```

Another way to do input handling is by checking if a key is pressed
using the `Window` subsystem:

```cpp
while(!Window::should_close())
{
  if (Window::is_key_pressed(Key::Escape))
    Window::close();
  if (Window::is_key_pressed(Key::W))
    acceleration.x = ACCELERATION;
  // ...
}
```

## Logger

Brenta uses a powerful logger that lives into its own repo,
[oak](https://github.com/San7o/oak). Check it out for a more detailed
look.

TODO

## Audio

The audio subsystem is very simple: there are audio streams and audio
files, you can play an audio file on a stream and stop it. Each stream
can play only one audio at a time, so you need to have multiple
streams if you want to play multiple sounds at the same time.

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


## Opengl

TODO
texture, shader, buffers...

## Rendering Objects

TODO
Camera, mesh, model, material, light

### Particles

TODO

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

### Text

TODO

The `brenta::text` subsystem allows you to render text on the screen. You can
set the font and font size of your text, and render it in the main loop like
this:
```cpp
Text::render_text("Hello OpenGL!", x, y, scale, glm::vec3(r, g, b));
```

## Renderer

TODO

## ECS

The ECS is a framework to organize your objects and how they
update. It is popular because it is arguably simple and elegant, and
(if implemented correctly) cache-friendly and parallelizable.

### World

Everything in the ECS exists in the `World`, you can think of it as
the state of everything that is happening.  The World contains
`Entities`, those are the most elemental things that exist.  You can
add `Components` to entities, which are their properties (like Health,
Position, Mesh). You interact with those components through `Systems`
by making `Queries` on their components. There are also `Resources`
that store global data. Uh that was quick, read it again if you need
it to.

You interact with the world via static methods of the `World`
class. The main loop should call `World::tick()`. At each tick, all
the systems will be called in the order they were added in the World
(the engine does not support multithreading yet).

```cpp
while(!Window::should_Close())
{
  if (Window::is_key_pressed(Key::Escape))
      Window::close();
        
  Gl::set_color(Color::gray());
  Gl::clear();

  // Run all systems
  World::tick();

  Window::poll_events();
  Window::swap_buffers();
}
```

### Entities

Entities are "objects" that exist in the ECS world. Practically, each
entity **is** an unique identifier that identifies the object.

To create an entity:

```cpp
Entity e = World::new_entity();
```

To remove an entity from the world and its components:

```cpp
World::remove_entity(e);
```

### Component

A `Component` is a piece of data that gets assigned to an Entity, you
can assign multiple components to an entity. This is essentially like
using composition, where the entity is associated with a set of
components. Unlike a regular class, all components are stored in a
database where they can be queried and iterated upon efficiently.

To define a component you need to extend the `Component` class:

```cpp
struct PhysicsComponent : Component
{
    float mass;
    float density;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    
    PhysicsComponent(float mass) : mass(mass) {}
};
```

To assign a component to an entity:

```cpp
World::add_component<PhysicsComponent>(entity, 10.0f);

// or

e.add_component<PhysicsComponent>(10.0f);
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
struct RenderSystem : System<ModelComponent, TransformComponent>
{
  // You need to define this function
  void run(std::vector<EntityId> matches) const override
  {
    if (matches.empty()) return;

    for (auto match : matches)
    {
      // Get the model component
      auto model_c = World::entity_to_component<ModelComponent>(match);
      auto my_model = model_c->mod;

      // Translate the model
      // ...

      my_model.draw(default_shader);
    }
  }
};

// Register this system
World::register_systems<RenderSystem>();
```

### Resources

Resources hold global data accessible via
`World::get_resource<name>()`.  You can define a Resource like so:

```c++
struct WireframeResource : Resource
{
  bool enabled;
  WireframeResource(bool e) : enabled(e) {}
};

World::add_resource<WireframeResource>(false);
```

Here is an high level simplified view of those objects:

![image](https://github.com/user-attachments/assets/d76b238d-56f1-4b57-8140-400af6ed1d23)


## Scene graph

TODO

### Scene

TODO

### Nodes

TODO

### Scripts

TODO

### Events

TODO

## AssetManager

TODO

## Hot Reloading

TODO

## App

TODO

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
- `src/`: engine sources
- `tests/`: engine Tests
- `utils/`
  - `docs-images`: images
  - `doxygen`: doxigen configuration files
  - `website`: files for website generation
