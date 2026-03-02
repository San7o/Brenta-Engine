// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/node_components/dir_light_node_component.hpp>
#include <brenta/renderer/renderer.hpp>

using namespace brenta;

void DirLightNodeComponent::update(float delta_time)
{
  (void) delta_time;
}

void DirLightNodeComponent::draw(const glm::mat4& world_matrix)
{
  (void) world_matrix;
  Renderer::submit_dir_light(this->light);
}
