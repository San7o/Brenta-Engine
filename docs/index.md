![](./html/images/brenta-engine-banner.png)

Hi and welcome to the **Brenta Engine** website! Brenta Engine is a
simple 3D engine written in modern C++/OpenGL using the Entity
Component System architecture. The engine was created by Giovanni
Santini in the summer of 2024, the name is inspired by the Brenta
Dolimites in the Italian Alps.

## The Submodules

The Engine is composed of several submodules, which are mostly
independent from each other:

* **[viotecs](https://github.com/San7o/viotecs)**: Entity Component
System
- **[oak](https://github.com/San7o/oak)**: engine logger
- **[valfuzz](https://github.com/San7o/valFuzz)**: testing
framework
- **brenta::window**: manages the window and the OpenGL context.
- **brenta::audio**: everything audio.
- **brenta::input**: manages the screen input using callbacks.
- **brenta::text**: text rendering.
- **brenta::engine**: manages the setup of the engine.

Additionally, the engine provides multiple classes for various
functionalities such as wrappers around opengl primitives, or managing
the camera or the time. Classes often provide a `Builder` to initalize
them nicely.

The classes are:

- **brenta::gl**: provides some useful OpenGL functions.
- **brenta::mesh**: a 3D openGL mesh.
- **brenta::model**: a 3D openGL model.
- **brenta::particle_emitter**: create and customize particles.
- **brenta::shader**: manages the shaders.
- **brenta::texture**: manages the textures.
- **brenta::types::translation**: manages the translations.
- **brenta::types::vao**: wrapper around the Vertex Array Objects.
- **brenta::types::buffer**: wrapper around the Buffers.
- **brenta::types::framebuffer**: framebuffer wrapper.
- **brenta::time**: manages the time.
- **brenta::camera**: manages the camera.
- **brenta::gui**: provides helper functions to ImGUI.

## Graphics

The engine uses OpenGL for the graphics and provides classes to
interact with opengl. The following features are already implemented:

- **Color Blending**
- **Cull Face**
- **Multisample**
- **Depth Test**
- **MSAA**
- **Vsync**
- **obj model loading**
- **Text rendering**
- **Lighting**: Ambient, Diffuse and Specular lighting.
- **Texture atlas**: texture atlas support.
- **GPU Particles**: particle system in the GPU.

Although the engine currently implements only basic graphics features,
it provides all the building blocks to create more complex
graphics. If you want to contribute additional features, check out the
[Contributing](./CONTRIBUTING.md)
page. [This](https://github.com/San7o/Brenta-Engine/issues/10) issue
tracks future features that would be nice to have.

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

## License

The engine is released under the MIT license. You can find the license
in the repository.
