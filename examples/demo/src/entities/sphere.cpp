// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/asset_manager.hpp>
#include <brenta/renderer/model.hpp>
#include <brenta/ecs/ecs.hpp>
#include <brenta/ecs/components/model_ecs_component.hpp>
#include <brenta/ecs/components/transform_ecs_component.hpp>
#include <brenta/logger.hpp>

#include <demo/components/physics.hpp>
#include <demo/components/sphere_collider.hpp>
#include <demo/entities.hpp>
#include <demo/utils.hpp>

#include <tenno/utility.hpp>

using namespace viotecs;
using namespace brenta;

void init_sphere_entity()
{
  auto default_material = get_default_material();
  if (!default_material)
  {
    ERROR("Demo: error getting default materia;");
    return;
  }

  //
  // Spere 1
  //

  auto transform1 = Transform()
    .translate(glm::vec3(2.0f, 1.0f, 0.0f));
  auto model1_builder =
    Model::Builder()
    .path("examples/assets/models/sphere/sphere.obj")
    .material(default_material);
  auto sphere1 = World::new_entity()
    .add_component<TransformEcsComponent>(transform1)
    .add_component<PhysicsEcsComponent>(1.0f,                        // mass
                                        1.0f,                        // density
                                        glm::vec3(0.0f, 0.0f, 0.0f), // velocity
                                        glm::vec3(-1.f, 0.0f, 0.0f), // acceleration
                                        true)                        // isElastic
    .add_component<SphereColliderEcsComponent>(1.0f)
    .add_component<ModelEcsComponent>(model1_builder);

  //
  // Spehere 2
  //

  auto transform2 = Transform()
    .translate(glm::vec3(-2.0f, 1.0f, 0.0f));
  auto model2_builder =
    Model::Builder()
    .path("examples/assets/models/sphere/sphere.obj")
    .material(default_material);
  auto sphere2 = World::new_entity()
    .add_component<TransformEcsComponent>(transform2)
    .add_component<PhysicsEcsComponent>(1.0f,                        // mass
                                        1.0f,                        // density
                                        glm::vec3(0.0f, 0.0f, 0.0f), // velocity
                                        glm::vec3(1.0f, 0.0f, 0.0f), // acceleration
                                        true)                        // isElastic
    .add_component<SphereColliderEcsComponent>(1.0f)
    .add_component<ModelEcsComponent>(model2_builder);
}
