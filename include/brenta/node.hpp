// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/script.hpp>
#include <brenta/transform.hpp>

#include <tenno/memory.hpp>
#include <tenno/vector.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace brenta
{

class NodeComponent;
  
//
// Node
// ----
//
// Nodes are the highest abstraction to repserent the scene. Each
// node has one parent and can have many children, creating a tree
// or a graph.
//
// When a node updates itself, it also updates its childrens; this
// propagates recursively down the tree. The same happens for drawing.
//
// The actual usefulness of a node cames with its components, which
// provide specialized functionalities to the node. Each node also
// has a lua script that it runs when it updates, to provide a more
// dynamic and simple way to write the Node's logic.
//
class Node
{
public:

  friend class Scene;

  Transform transform;
  
  Node() = default;

  // Update logic (animations, movement, AI...)
  void update(float delta_time);

  // Render frame
  void draw();

private:

  tenno::vector<tenno::shared_ptr<NodeComponent>>  components;
  std::optional<Script>                            script;

  std::optional<tenno::weak_ptr<Node>>      parent;
  tenno::vector<tenno::shared_ptr<Node>>    children;

  glm::mat4 world_matrix = glm::mat4(1.0f);

  void update_world_matrix();
  
};

class NodeComponent
{
public:

  friend class Scene;

  tenno::weak_ptr<Node>     owner;

  virtual ~NodeComponent() = default;
  
  virtual void update(float delta_time)            = 0;
  virtual void draw(const glm::mat4& world_matrix) = 0;

};

} // namespace brenta
