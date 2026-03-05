// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/asset.hpp>
#include <brenta/renderer/phong.hpp>
#include <brenta/ecs/ecs.hpp>
#include <brenta/ecs/components/point_light_ecs_component.hpp>
#include <brenta/ecs/components/transform_ecs_component.hpp>

#include <demo/entities.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace viotecs;
using namespace brenta;

void init_point_light_entity()
{
  auto transform = Transform()
    .translate(glm::vec3(0.0f, 10.0f, 0.0f))
    .scale(glm::vec3(0.5f));

  auto phong_point =
    PhongPointLight()
    .set_strength(1.8f);
  auto phong_point_ptr =
    tenno::make_shared<PhongPointLight>(tenno::move(phong_point));

  auto light = World::new_entity()
    .add_component<PointLightEcsComponent>(phong_point_ptr)
    .add_component<TransformEcsComponent>(transform);
}
