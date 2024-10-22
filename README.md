![Brenta-Engine-Banner](./assets/brenta-engine-banner.png)

**Brenta Engine** is a simple 3D engine written in modern C++/OpenGL
using the Entity Component System architecture. The engine was
created by Giovanni Santini in the summer of 2024, the name is
inspired by the Brenta Dolimites in the Italian Alps.

Check out `GUIDE.md` for a quick introduction on how the engine works!

<h2 align=center>  Related Projects </h2>

The engine also features the following sub projects:

- [oak](https://github.com/San7o/oak): feature-rich, thread-safe, Brenta Engine's logger.
- [viotecs](https://github.com/San7o/viotecs): the engine's official ECS.
- [valFuzz](https://github.com/San7o/valFuzz): multi-threaded testing and fuzzing library for the engine.


<h1 align=center>  Features </h1>

The engine is composed of many submodules independent from each other, those are:
- `Screen`: manages the window and the OpenGL context.
- `Audio`: everything audio.
- `Input`: manages the screen input using callbacks.
- `Text`: text rendering.
- `Engine`: manages the setup of the engine.
- `Particles`: generate parametrized particles.

In addition to those, Brenta comes with many utility functions and classes, to get
a detailed look at the engine, please visit the [documentation](https://san7o.github.io/brenta-engine-documentation/v1.1/), **every class and function is highly documented.**

### Ergonomic APIs
```c++
engine game = engine::builder()
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
```

### Model Loading
```c++
model my_model("assets/models/backpack/backpack.obj");
```
![image](https://github.com/user-attachments/assets/e4facf89-4256-4ecb-ae0e-9340aaf7b372)


### Particles

```c++
particle_emitter emitter =
    particle_emitter::builder()
        .set_camera(&camera)
        .set_starting_position(glm::vec3(0.0f, 0.0f, 0.0f))
        .set_starting_velocity(glm::vec3(0.0f, 5.0f, 0.0f))
        .set_starting_spread(glm::vec3(3.0f, 10.0f, 3.0f))
        .set_starting_time_to_live(0.5f)
        .set_num_particles(1000)
        .set_spawn_rate(0.01f)
        .set_scale(1.0f)
        .set_atlas_path("assets/textures/particle_atlas.png")
        .set_atlas_width(8)
        .set_atlas_height(8)
        .set_atlas_index(3)
        .build();
```

![particles_short](https://github.com/user-attachments/assets/27d5ac09-00ce-4379-bf47-d16c24de9508)

### Texture Animation using an Atlas
![texture_atlas_short](https://github.com/user-attachments/assets/1a379fa5-741b-4087-a078-68a86a1fea98)

### 3D Camera
https://github.com/user-attachments/assets/f0ea502c-dc9e-4609-8322-641eb7d65a77

Also collisions, lighting, text and audio!

<h1 align=center> ECS </h1>
Brenta Engine features an Entity Component System architecture. The ECS is a design pattern that allows you to structure your code in a way that is more modular and scalable.

### Entities
Entities are objects in the game, It's just an ID:
```c++
entity entity = world::new_entity();
```
### Components
Components are pieces of data that are attached to an entity:
```c++
struct physics_component : component {
    float mass;
    float density;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    physics_component() {}
};

// Somewhere
world::add_component<physics_component>(entity, physics_component);
```
### Systems
Systems are functions that operate on entities with specific components. They
are called at each game tick by the `World`:
```c++
struct fps_system : system<none> {
    void run(std::vector<entity_t> _) const override {
        text::render_text("FPS: " + std::to_string(Time::GetFPS()), 25.0f, 25.0f,
                         0.35f, glm::vec3(1.0f, 0.9f, 0.0f));
    }
};

// Somewhere
REGISTER_SYSTEMS(fps_system);
```
<h1 align=center> Building </h1>

All instructions to build the demo game are in [BUILD](./BUILD.md), there are also instructions on
how to [build unit tests](./tests/README.md) and how to [build examples](./examples/README.md).

<h1 align=center> Contributing </h1>

Even if this is not an advanced graphics engine, It provides many opportunities for you to practice and improve your programming skills. You can look at the current issues to get an idea of where the engine is going and where It needs help. As the engine grows, help from contributors will be essential to keep the project alive.

Check out [CONTRIBUTING](./CONTRIBUTING.md) for information on how to contribute.

The engine is licensed under [MIT](https://en.wikipedia.org/wiki/MIT_License) license.
