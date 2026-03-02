// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/script.hpp>
#include <brenta/node_component.hpp>
#include <brenta/transform.hpp>

#include <tenno/memory.hpp>
#include <tenno/vector.hpp>

namespace brenta
{

class Node
{
public:

  friend class Scene;
  
  Node() = default;

  void         set_local(Transform local);
  Transform   &get_local();

  // Update logic (animations, movement, AI...)
  void update(float delta_time);

  // Render frame
  void draw();

private:

  tenno::vector<tenno::shared_ptr<NodeComponent>>  components;
  std::optional<Script>                            script;

  std::optional<tenno::weak_ptr<Node>>      parent;
  tenno::vector<tenno::shared_ptr<Node>>    children;

  Transform local;
  glm::mat4 world_matrix = glm::mat4(1.0f);

  void update_world_matrix();
  
};

} // namespace brenta
