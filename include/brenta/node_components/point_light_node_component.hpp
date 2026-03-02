// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/node_component.hpp>
#include <brenta/renderer/point_light.hpp>

#include <tenno/memory.hpp>

namespace brenta
{

class PointLightNodeComponent : public NodeComponent
{
public:

  PointLightNodeComponent() = default;
  PointLightNodeComponent(tenno::shared_ptr<PointLight> l)
    : light(l) {}
  
  void update(float delta_time) override;
  void draw(const glm::mat4& world_matrix) override;

private:

  tenno::shared_ptr<PointLight> light;
  
};

} // namespace brenta
