// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/asset_manager.hpp>
#include <brenta/renderer/phong.hpp>
#include <brenta/ecs/ecs.hpp>
#include <brenta/ecs/components/dir_light_ecs_component.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace viotecs;
using namespace brenta;

void init_directional_light_entity()
{
  auto phong_dir =
    PhongDirLight()
    .set_strength(0.8f);
  auto phong_dir_ptr =
    tenno::make_shared<PhongDirLight>(tenno::move(phong_dir));
  
  auto light = World::new_entity()
    .add_component<DirLightEcsComponent>(phong_dir_ptr);

  return;
}
