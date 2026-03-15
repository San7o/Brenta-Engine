// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/node.hpp>

#include <tenno/memory.hpp>

namespace brenta
{

class PointLight;
  
class PointLightNodeComponent : public NodeComponent
{
public:

  tenno::shared_ptr<PointLight> light;
  
  PointLightNodeComponent() = default;
  PointLightNodeComponent(tenno::shared_ptr<PointLight> l)
    : light(l) {}
  
  void update(float delta_time) override;
  void draw(const glm::mat4& world_matrix) override;

  
};

} // namespace brenta
