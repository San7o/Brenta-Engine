// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/transform.hpp>
#include <brenta/renderer/model.hpp>
#include <brenta/renderer/renderer.hpp>
#include <brenta/ecs/ecs.hpp>

#include <tenno/memory.hpp>

namespace brenta
{

class ModelEcsComponent : public viotecs::Component
{
public:

  tenno::shared_ptr<Model> model    = nullptr;

  ModelEcsComponent() = default;
  ModelEcsComponent(tenno::shared_ptr<Model> m)
    : model(m) {}
  ModelEcsComponent(Model&& m)
  {
    this->model = tenno::make_shared<Model>(tenno::move(m));
  }
  ModelEcsComponent(Model::Builder& m)
  {
    this->model = tenno::make_shared<Model>(m.build());
  }
  
  void draw(const glm::mat4& world_matrix)
  {
    Renderer::submit({
        world_matrix,
        this->model
      });
  }

};

} // namespace brenta
