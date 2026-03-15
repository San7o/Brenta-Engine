// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/node_components/particle_emitter_node_component.hpp>
#include <brenta/renderer/opengl/framebuffer.hpp>

using namespace brenta;

void ParticleEmitterNodeComponent::update(float delta_time)
{
  if (this->emitter)
    this->emitter->update(delta_time);
}

void ParticleEmitterNodeComponent::draw(const glm::mat4& world_matrix)
{
  (void) world_matrix;
  
  if (!this->emitter)
    return;

  if (auto f = this->fb.lock())
    this->emitter->render(f->width, f->height);
}
