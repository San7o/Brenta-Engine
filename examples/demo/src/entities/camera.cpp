// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/ecs/components/camera_ecs_component.hpp>
#include <demo/entities.hpp>

void init_camera_entity(tenno::shared_ptr<brenta::Camera> c)
{
  auto camera_e = viotecs::World::new_entity();
  camera_e.add_component<brenta::CameraEcsComponent>(c);

  return;
}
