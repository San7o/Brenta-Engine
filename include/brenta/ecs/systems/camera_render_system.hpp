// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#ifndef BRENTA_NO_ECS

#include <brenta/ecs/ecs.hpp>
#include <brenta/renderer/opengl/framebuffer.hpp>
#include <brenta/ecs/components/camera_ecs_component.hpp>

#include <vector>

namespace brenta
{

class CameraRenderSystem : public viotecs::System<CameraEcsComponent>
{
public:
  
  void run(std::vector<viotecs::EntityId> entities) const override
  {
    for (auto& e : entities)
    {
      auto camera =
        viotecs::World::entity_to_component<CameraEcsComponent>(e);

      if (const auto& f = camera->fb.lock())
          Renderer::set_camera(*camera->camera,
                               f->width, f->height);
    }
  }
};
  
} // namespace brenta

#endif // BRENTA_NO_ECS
