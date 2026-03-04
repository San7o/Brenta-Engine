// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <tenno/memory.hpp>

namespace brenta
{

class Node;
class Scene;
  
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
