// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/node_components/text_node_component.hpp>
#include <brenta/renderer/renderer.hpp>

using namespace brenta;

void TextNodeComponent::update(float delta_time)
{
  (void) delta_time;
}

void TextNodeComponent::draw(const glm::mat4& world_matrix)
{
  (void) world_matrix;
  Renderer::submit_text(this->text);
}
