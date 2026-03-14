// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/node.hpp>
#include <brenta/renderer/particles.hpp>
#include <brenta/renderer/opengl/framebuffer.hpp>

#include <tenno/memory.hpp>

namespace brenta
{

class ParticleEmitterNodeComponent : public NodeComponent
{
public:

  tenno::shared_ptr<ParticleEmitter> emitter;
  tenno::weak_ptr<FrameBuffer>       fb;

  ParticleEmitterNodeComponent() = default;
  ParticleEmitterNodeComponent(tenno::shared_ptr<ParticleEmitter> e,
                               tenno::weak_ptr<FrameBuffer> fb)
    : emitter(e), fb(fb) {}
  ParticleEmitterNodeComponent(ParticleEmitter&& e, tenno::weak_ptr<FrameBuffer> fb)
  {
    this->emitter = tenno::make_shared<ParticleEmitter>(tenno::move(e));
    this->fb = fb;
  }
  ParticleEmitterNodeComponent(ParticleEmitter::Builder& builder,
                               tenno::weak_ptr<FrameBuffer> fb)
  {
    this->emitter = tenno::make_shared<ParticleEmitter>(builder.build());
    this->fb = fb;
  }
  
  void update(float delta_time) override;
  void draw(const glm::mat4& world_matrix) override;
  
};

} // namespace brenta
