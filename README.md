![Brenta-Engine-Banner](./utils/docs-images/brenta-engine-banner.png)

**Brenta Engine** is a simple 3D graphics engine written in modern
C++/OpenGL using a hybrid scene-graph and Entity Component System
architecture.

Check out [DESIGN.md](./docs/DESIGN.md) for an overview of
the engine's architecture, and [GUIDE.md](./docs/GUIDE.md) for an
introduction to its API.

<h2 align=center>  Features </h2>

- hot reloading
- GPU particles
- loading TrueType fonts, textures, audio and .obj meshes
- central asset management
- scene graph
- ECS
- lua node scripting
- GUI using ImGui
- signals
- point and directional lights
- window creation
- input management with callbacks
- audio playback
- powerful logging
- openGL RAII abstractions

To get a detailed look at the engine, please visit the
[website](https://san7o.github.io/Brenta-Engine/) and code
[documentation](https://san7o.github.io/Brenta-Engine/annotated.html).

The engine also features the following sub projects:

- [oak](https://github.com/San7o/oak): feature-rich, thread-safe, Brenta Engine's logger.
- [viotecs](https://github.com/San7o/viotecs): the engine's official ECS.
- [tenno](https://github.com/San7o/tenno-tl): custom standard library
- [valFuzz](https://github.com/San7o/valFuzz): multi-threaded testing and fuzzing library for the engine.
- [san7o.github.io/Brenta-Engine/](https://san7o.github.io/Brenta-Engine/): html website


<h1 align=center> Screenshots </h1>

<div align="center">
  <img src="docs/screenshot1.jpg" />
  <img src="docs/screenshot2.jpg" />
</div>

<h1 align=center> Building </h1>

The engine can be used as a library, there are also many
[examples](./examples) that you can build. Follow the
[BUILD](./docs/BUILD.md) document to get started.

The engine is licensed under [MIT](./LICENSE) license.
