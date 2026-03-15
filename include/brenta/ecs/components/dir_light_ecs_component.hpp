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

class DirLightEcsComponent : public viotecs::Component
{
public:
  
  tenno::shared_ptr<DirLight> light   = nullptr;

  DirLightEcsComponent() = default;
  DirLightEcsComponent(tenno::shared_ptr<DirLight> l)
  {
    this->light = l;
  }
  template<typename U>
  DirLightEcsComponent(tenno::shared_ptr<U> l)
  {
    this->light = l;
  }

  inline void draw()
  {
    Renderer::submit_dir_light(this->light);
  }
  
};
  
} // namespace brenta

#endif // BRENTA_NO_ECS
