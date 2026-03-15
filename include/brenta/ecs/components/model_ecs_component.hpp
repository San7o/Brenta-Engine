// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#ifndef BRENTA_NO_ECS

#include <brenta/renderer/model.hpp>
#include <brenta/renderer/renderer.hpp>
#include <brenta/ecs/ecs.hpp>

#include <tenno/memory.hpp>

namespace brenta
{

class ModelEcsComponent : public viotecs::Component
{
public:

  tenno::shared_ptr<Model> model         = nullptr;
  bool                     transparent   = false;

  ModelEcsComponent() = default;
  ModelEcsComponent(tenno::shared_ptr<Model> m, bool transparent = false)
    : model(m), transparent(transparent) {}
  ModelEcsComponent(Model&& m, bool transparent = false)
    : transparent(transparent)
  {
    this->model = tenno::make_shared<Model>(tenno::move(m));
  }
  ModelEcsComponent(Model::Builder& m, bool transparent = false)
    : transparent(transparent)
  {
    this->model = tenno::make_shared<Model>(m.build());
  }
  
  inline void draw(const glm::mat4& world_matrix)
  {
    Renderer::submit({
        world_matrix,
        this->model
      },
      this->transparent);
  }

};

} // namespace brenta

#endif // BRENTA_NO_ECS
