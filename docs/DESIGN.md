# Design

Game engines are complex pieces of software. They provide an interface
to define logic, render graphics, and access system resources like
audio and input, as well as providing a cross-platform abstraction to
the developer.

Here is an high-level overview of the main objects Brenta provides:

![brenta-picture](./brenta-picture.png)

I wanted to write my own game engine primarely out of fascination and
curiosity to understand how these big systems are designed and
implemented. I found out that writing a game engine has a lot in
common with writing an operating system. You are working with audio,
files, video, network and the GPU. A game engine is essentially a
realtime system since you need to compute logic and render the frame
in under 16ms to run at 60 FPS, hence you have to understand how the
CPU and memory works in order to optimize it. Obviously game engines
and an operating system diverge in many other ways, for example an
operating system should manage virtualization and security, while a
game engine has to manage game logic and physics. Still, I think the
parallelism is clear: you need to interface and reason about many
types of systems and devices, which makes this project interesting to
me.

![engine-and-os](./engine-and-os.png)


Brenta engine is divided in subsystems, each one has different
responsibilities and provides certain abstractions. The most important
subsystems are the Rendering, which manages things like the scene and
render commands, and the Entity Component System (ECS) which manages
game logic.

![brenta-subsystems](./brenta-subsystems.png)

## Renderer

The renderer provides a set of abstraction for working with geometry
and lights in order to render a frame on the screen (or to a
framebuffer).

The actual rendering algorithm deserves its own book. There are many
rendering techniques available, the most popular ones are
rasterization which is common in realtime rendering, and ray tracing
which is powerful but slower so it is often used for offline rendering
(but modern GPUs now provide raytracing support on hardware to speed
this up).  Brenta uses rasterization, which means projecting each
surface to the screen and calculating the color of each pixel by
interpolating each one of them (parallelized on the GPU).

This "projection" is achieved by multiplying together three matrices:
the `model` or `world` matrix which translates a vertex to its
position in world space, the `view` matrix which shift and rotates the
world based on the camera position (If the camera moves 5 feet to the
right, it’s mathematically the same as moving the entire world 5 feet
to the left), and `projection` matrix which applies perspective and
field-of-view; this ultimately maps the vertices inside a cube called
the Canonical Cube where the GPU can work with.

Another huge topic is lighting. Brenta implements the [Phong
reflection
model](https://en.wikipedia.org/wiki/Phong_reflection_model) but
provides the abstractions necessary to integrate other methods.

## Scene

There are many ways to define a scene. Brenta supports both the
scene-graph architecture, commonly used in Godot, Unity and Unreal,
and the ECS (Entity Component System) architecture like in
[Bevy](https://github.com/bevyengine/bevy/).

### Scene graph

The scene is a tree of nodes where each node has a transform, and
may have a model, a directional light, any number of point lights and
other nodes (children). All transforms are relative to the transform
of their parent; when a node is updated, all its children are updated
too.

![scene](./brenta-scene.png)

Here is an high-level picture that shows the main classes used in the
scene graph, where arrows going down mean the parent contains one or
more children:

![renderer-design](./brenta-renderer.png)

### Ecs

The Entity Component System architecture is used to manage all logic
of a videogame. Check out [viotecs](https://github.com/San7o/viotecs)
for more information.

![image](https://github.com/user-attachments/assets/d76b238d-56f1-4b57-8140-400af6ed1d23)
