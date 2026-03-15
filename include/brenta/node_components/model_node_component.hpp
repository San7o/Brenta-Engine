// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me


#pragma once

#include <brenta/node.hpp>
#include <brenta/renderer/model.hpp>

#include <tenno/memory.hpp>

namespace brenta
{

class ModelNodeComponent : public NodeComponent
{
public:

  tenno::shared_ptr<Model>   model;
  bool                       transparent = false;

  ModelNodeComponent() = default;
  ModelNodeComponent(tenno::shared_ptr<Model> m, bool transparent = false)
    : model(m), transparent(transparent) {}
  ModelNodeComponent(Model&& m, bool transparent = false)
    : transparent(transparent)
  {
    this->model = tenno::make_shared<Model>(tenno::move(m));
  }
  ModelNodeComponent(Model::Builder& m, bool transparent = false)
    : transparent(transparent)
  {
    this->model = tenno::make_shared<Model>(m.build());
  }
  
  void update(float delta_time) override;
  void draw(const glm::mat4& world_matrix) override;
  
};

} // namespace brenta
