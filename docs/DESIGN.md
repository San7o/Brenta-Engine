# Design

THIS DOCUMENT IS JUST A SKETCH AND NEEDS TO BE UPDATED

Game engines are complex pieces of software. They provide some way to
define logic, render graphics, and access system resources like audio
and input, as well as providing a cross-platform interface.

Brenta engine is divided in subsystems, each one has different
responsibilities and provides certain abstractions. The most important
subsystems are the rendering, which manages things like models and
textures, and the entity component system which manages logic.

![brenta-picture](./brenta-picture.png)


## Renderer

The renderer provides a set of abstraction for working with geometry
and lights in order to render a frame on the screen (or a
framebuffer).

Here is an high-level picture that shows the main classes of the
renderer subsystem, where arrows going down mean the parent contains
one or more children:

![renderer-design](./brenta-renderer.png)

The scene is a tree of nodes where each node has a transform, and
may have a model, a directional light, any number of point lights and
other nodes (children). All transforms are relative to the transform
of their parent; when a node is updated, all its children are updated
too.

![scene](./brenta-scene.png)

The actual rendering part deserves its own book. There are many
rendering techniques available, the most popular ones are
rasterization and ray tracing. Brenta uses rasterization, which means
projecting each surface to the screen and calculating the color of
each pixel by interpolating each one of them (parallelized on the
GPU).

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

## Ecs

The Entity Component System architecture is used to manage all logic
of a videogame. Check out [viotecs](https://github.com/San7o/viotecs)
for more information.


![image](https://github.com/user-attachments/assets/d76b238d-56f1-4b57-8140-400af6ed1d23)
