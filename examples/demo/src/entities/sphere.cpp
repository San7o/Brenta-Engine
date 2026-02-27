// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/asset_manager.hpp>
#include <brenta/renderer/model.hpp>

#include <demo/components/model.hpp>
#include <demo/components/physics.hpp>
#include <demo/components/sphere_collider.hpp>
#include <demo/components/transform.hpp>
#include <demo/entities/sphere.hpp>

#include <viotecs/viotecs.hpp>
#include <tenno/utility.hpp>

#include "../../../../src/renderer/shaders/c/phong_vs.c"
#include "../../../src/renderer/shaders/c/phong_fs.c"

using namespace viotecs;
using namespace brenta;

void init_sphere_entity()
{
  //
  // Spere 1
  //

  auto shader = AssetManager::get_shader("default_shader");
  if (!shader)
  {
    shader = AssetManager::new_shader("default_shader", {
        { Shader::Type::Vertex,   phong_vs },
        { Shader::Type::Fragment, phong_fs } });
  }
  Model m1 = Model::Builder()
    .path("examples/assets/models/sphere/sphere.obj")
    .transform(Transform()
               .translate(glm::vec3(2.0f, 1.0f, 0.0f)))
    .build();

  auto sphere1 = World::new_entity()
    .add_component<TransformComponent>(m1.get_transform())
    .add_component<PhysicsComponent>(1.0f,                        // mass
                                     1.0f,                        // density
                                     glm::vec3(0.0f, 0.0f, 0.0f), // velocity
                                     glm::vec3(-1.f, 0.0f, 0.0f), // acceleration
                                     true)                        // isElastic
    .add_component<SphereColliderComponent>(1.0f)
    .add_component<ModelComponent>(tenno::move(m1), 32.0f, shader);

  //
  // Spehere 2
  //

  Model m2 = Model::Builder()
    .path("examples/assets/models/sphere/sphere.obj")
    .transform(Transform()
               .translate(glm::vec3(-2.0f, 1.0f, 0.0f)))
    .build();
  
  auto sphere2 = World::new_entity()
    .add_component<TransformComponent>(m2.get_transform())
    .add_component<PhysicsComponent>(1.0f,                        // mass
                                     1.0f,                        // density
                                     glm::vec3(0.0f, 0.0f, 0.0f), // velocity
                                     glm::vec3(1.0f, 0.0f, 0.0f), // acceleration
                                     true)                        // isElastic
    .add_component<SphereColliderComponent>(1.0f)
    .add_component<ModelComponent>(tenno::move(m2), 32.0f, shader);
}
