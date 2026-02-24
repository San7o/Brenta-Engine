![Brenta-Engine-Banner](./utils/docs-images/brenta-engine-banner.png)

**Brenta Engine** is a simple 3D engine written in modern C++/OpenGL
using a hybrid scene-graph and Entity Component System
architecture. The engine was created by Giovanni Santini in the summer
of 2024, the name is inspired by the Brenta Dolimites in the Italian
Alps.

Check out [GUIDE.md](./docs/GUIDE.md) for a quick introduction on how
the engine works, and [DESIGN.md](./docs/DESIGN.md) for an overview of
the engine's internals.

<p align="center">
  <img src="https://san7o.github.io/Brenta-Engine/brenta-picture.png" />
</p>

<h2 align=center>  Features </h2>

The engine is composed of many subsystems like `Window`, `Input`,
`Audio`, `Engine`, `Logger`, `Ecs` as well as a custom opengl RAII
objects and a scene-graph.

To get a detailed look at the engine, please visit the
[website](https://san7o.github.io/Brenta-Engine/) and code
[documentation](https://san7o.github.io/Brenta-Engine/annotated.html),
**every class and function is highly documented.**

The engine also features the following sub projects:

- [oak](https://github.com/San7o/oak): feature-rich, thread-safe, Brenta Engine's logger.
- [viotecs](https://github.com/San7o/viotecs): the engine's official ECS.
- [valFuzz](https://github.com/San7o/valFuzz): multi-threaded testing and fuzzing library for the engine.
- [san7o.github.io/Brenta-Engine/](https://san7o.github.io/Brenta-Engine/): html website


### Modular APIs

```c++
auto engine = Engine::Builder()
  .with(Logger::Builder()
        .level(Logger::Level::debug)
        .file("/tmp/brenta-logs"))
  .with(Window::Builder()
        .title("brenta demo")
        .width(800)
        .height(600)
        .vsync()
        .msaa())
  .with(Gl::Builder()
        .blending()
        .cull_face()
        .multisample()
        .depth_test())
  .build();
```

### Model Loading

```c++
auto model = Model::Builder()
    .path("assets/models/backpack/backpack.obj")
    .transform(Transform()
               .translate(glm::vec3(1.0f, 5.0f, 2.0f))
               .scale(glm::vec3(2.0f, 1.0f, 1.0f)))
    .build();
```

![image](https://github.com/user-attachments/assets/e4facf89-4256-4ecb-ae0e-9340aaf7b372)


### GPU Particles

```c++
auto emitter = ParticleEmitter::Builder()
        .with_camera(&camera)
        .starting_position(glm::vec3(0.0f, 0.0f, 0.0f))
        .starting_velocity(glm::vec3(0.0f, 5.0f, 0.0f))
        .starting_spread(glm::vec3(3.0f, 10.0f, 3.0f))
        .starting_time_to_live(0.5f)
        .num_particles(1000)
        .spawn_rate(0.01f)
        .scale(1.0f)
        .atlas_path("assets/textures/particle_atlas.png")
        .atlas_width(8)
        .atlas_height(8)
        .atlas_index(3)
        .build();
```

![particles_short](https://github.com/user-attachments/assets/27d5ac09-00ce-4379-bf47-d16c24de9508)

### Texture Animation using an Atlas

![texture_atlas_short](https://github.com/user-attachments/assets/1a379fa5-741b-4087-a078-68a86a1fea98)

### 3D Camera

```cpp
auto camera = Camera::Builder()
  .projection_type(Camera::ProjectionType::Perspective)
  .position(Camera::Spherical::Builder()
            .center({0.0f, 2.0f, 0.0f})
            .phi(1.25f)
            .theta(1.25f)
            .radius(30.0f)
            .build())
  .fov(45.0f)
  .build();
```

https://github.com/user-attachments/assets/f0ea502c-dc9e-4609-8322-641eb7d65a77

Also collisions, lighting, text and audio!

<h1 align=center> ECS </h1>

Brenta Engine features an Entity Component System architecture. The
ECS is a design pattern that allows you to structure your code in a
way that is more modular and scalable.

### Entities

Entities are objects in the game, it's just an ID. The `entity` class
helps you manage the entity, for example by attaching a component to
that entity, or removing the entity from the world.

```c++
Entity e = World::new_entity();
```

### Components

Components are pieces of data that are attached to an entity:

```c++
struct PhysicsComponent : Component {
    float mass;
    float density;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    PhysicsComponent(float mass) : mass(mass) {}
};

// Somewhere
e.add_component<PhysicsComponent>(10.0f);
```

### Systems

Systems are functions that operate on entities with specific components. They
are called at each game tick by the `World`:

```c++
struct FpsSystem : System<None> {
    void run(std::vector<entity_t> _) const override {
        text::render_text("FPS: " + std::to_string(time::get_fps()), 25.0f, 25.0f,
                         0.35f, glm::vec3(1.0f, 0.9f, 0.0f));
    }
};

// To register the system
World::register_systems<FpsSystem>();
```

<h1 align=center> Building </h1>

All instructions to build the demo game are in [BUILD](./docs/BUILD.md),
there are also instructions on how to [build unit tests](./tests/README.md)
and how to [build examples](./examples/README.md).

<h1 align=center> Contributing </h1>

Even if this is not an advanced graphics engine, it provides many
opportunities for you to practice and improve your programming
skills. You can look at the current issues to get an idea of where the
engine is going and where it needs help. As the engine grows, help
from contributors will be essential to keep the project alive.

Check out [CONTRIBUTING](./docs/CONTRIBUTING.md) for information on
how to contribute.

The engine is licensed under [MIT](./LICENSE) license.
