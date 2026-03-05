// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/node.hpp>
#include <brenta/renderer/particles.hpp>

#include <tenno/memory.hpp>

namespace brenta
{

class ParticleEmitterNodeComponent : public NodeComponent
{
public:

  tenno::shared_ptr<ParticleEmitter> emitter;

  ParticleEmitterNodeComponent() = default;
  ParticleEmitterNodeComponent(tenno::shared_ptr<ParticleEmitter> e)
    : emitter(e) {}
  ParticleEmitterNodeComponent(ParticleEmitter&& e)
  {
    this->emitter = tenno::make_shared<ParticleEmitter>(tenno::move(e));
  }
  ParticleEmitterNodeComponent(ParticleEmitter::Builder& builder)
  {
    this->emitter = tenno::make_shared<ParticleEmitter>(builder.build());
  }
  
  void update(float delta_time) override;
  void draw(const glm::mat4& world_matrix) override;
  
};

} // namespace brenta
