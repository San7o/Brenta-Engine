// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/node.hpp>
#include <brenta/renderer/dir_light.hpp>

#include <tenno/memory.hpp>

namespace brenta
{

class DirLightNodeComponent : public NodeComponent
{
public:

  tenno::shared_ptr<DirLight> light;
  
  DirLightNodeComponent() = default;
  DirLightNodeComponent(tenno::shared_ptr<DirLight> l)
    : light(l) {}
  
  void update(float delta_time) override;
  void draw(const glm::mat4& world_matrix) override;

};

} // namespace brenta

