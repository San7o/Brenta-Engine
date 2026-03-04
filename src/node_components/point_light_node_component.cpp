// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/node_components/point_light_node_component.hpp>
#include <brenta/renderer/renderer.hpp>

using namespace brenta;

void PointLightNodeComponent::update(float delta_time)
{
  (void) delta_time;
}

void PointLightNodeComponent::draw(const glm::mat4& world_matrix)
{
  auto& pos = this->light->position;
  pos = glm::vec3(world_matrix[3]);
  Renderer::submit_point_light(this->light);
}
