// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/node_components/model_node_component.hpp>
#include <brenta/renderer/renderer.hpp>

using namespace brenta;

void ModelNodeComponent::update(float delta_time)
{
  (void) delta_time;
}

void ModelNodeComponent::draw(const glm::mat4& world_matrix)
{
  Renderer::submit({world_matrix, this->model}, this->transparent);
}
