![](./html/images/brenta-engine-banner.png)

**Brenta Engine** is a simple 3D graphics engine written in modern
C++/OpenGL using a hybrid scene-graph and Entity Component System
architecture.

Check out [DESIGN.md](./DESIGN.md) for an overview of
the engine's architecture, and [GUIDE.md](./GUIDE.md) for an
introduction to its API.

<h2 align=center>  Features </h2>

- configurable rendering pipeline
- node graph
- ECS
- lua node scripting
- hot reloading
- GPU particles
- central asset management (fonts, textures, meshes...)
- point and directional lights
- GUI using ImGui
- signals
- skyboxes
- window management
- post processing
- audio loading and playback
- input management with callbacks
- powerful logging
- openGL RAII abstractions
- custom standard library
- lots of examples

The engine also features the following sub projects:

- [oak](https://github.com/San7o/oak): feature-rich, thread-safe, Brenta Engine's logger.
- [viotecs](https://github.com/San7o/viotecs): the engine's official ECS.
- [tenno](https://github.com/San7o/tenno-tl): custom standard library
- [valFuzz](https://github.com/San7o/valFuzz): multi-threaded testing and fuzzing library for the engine.
- [san7o.github.io/Brenta-Engine/](https://san7o.github.io/Brenta-Engine/): html website


<h2 align=center> Screenshots </h2>

<div align="center">
  <img src="demo-screenshot.png" />
  <img src="screenshot1.jpg" />
  <img src="screenshot2.jpg" />
</div>

<h2 align=center> Building </h2>

The engine can be used as a library, there are also many
[examples](https://github.com/San7o/Brenta-Engine/tree/main/examples)
that you can build. Follow the [BUILD](./BUILD.md) document to get
started.

The engine is licensed under [MIT](https://mit-license.org/) license.
