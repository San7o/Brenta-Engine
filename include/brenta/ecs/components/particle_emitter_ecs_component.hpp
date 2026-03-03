// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#ifndef BRENTA_NO_ECS

#include <brenta/renderer/particles.hpp>
#include <brenta/ecs/ecs.hpp>

#include <tenno/memory.hpp>

namespace brenta
{

class ParticleEmitterEcsComponent : public viotecs::Component
{
public:
  
  tenno::shared_ptr<ParticleEmitter> emitter   = nullptr;

  ParticleEmitterEcsComponent() = default;
  ParticleEmitterEcsComponent(tenno::shared_ptr<ParticleEmitter> e)
    : emitter(e) {}
  ParticleEmitterEcsComponent(ParticleEmitter&& e)
  {
    this->emitter = tenno::make_shared<ParticleEmitter>(tenno::move(e));
  }
  ParticleEmitterEcsComponent(ParticleEmitter::Builder &builder)
  {
    this->emitter = tenno::make_shared<ParticleEmitter>(builder.build());
  }

};
  
} // namespace brenta

#endif // BRENTA_NO_ECS
