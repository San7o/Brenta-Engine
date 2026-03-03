// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#ifndef BRENTA_NO_ECS

#include <brenta/ecs/ecs.hpp>
#include <brenta/ecs/components/particle_emitter_ecs_component.hpp>

#include <vector>

namespace brenta
{

class ParticleEmitterSystem : public viotecs::System<ParticleEmitterEcsComponent>
{
public:
  
  void run(std::vector<viotecs::EntityId> entities) const override
  {
    for (auto& entity : entities)
    {
      auto e = viotecs::Entity(entity);
      auto emitter = e.get_component<ParticleEmitterEcsComponent>();
      emitter->emitter->update(Window::get_time().get_delta());
      emitter->emitter->render();
    }
  }
};

} // namespace brenta

#endif // BRENTA_NO_ECS
