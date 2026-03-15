// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#ifndef BRENTA_NO_ECS

#include <brenta/renderer/renderer.hpp>
#include <brenta/ecs/ecs.hpp>

#include <tenno/memory.hpp>

namespace brenta
{

class PointLightEcsComponent : public viotecs::Component
{
public:
  
  tenno::shared_ptr<PointLight> light   = nullptr;

  PointLightEcsComponent() = default;
  PointLightEcsComponent(tenno::shared_ptr<PointLight> l)
  {
    this->light = l;
  }

  inline void draw(const Transform& t)
  {
    auto& pos = this->light->position;
    pos = t.get_pos();
    Renderer::submit_point_light(this->light);
  }
  
};
  
} // namespace brenta

#endif // BRENTA_NO_ECS
