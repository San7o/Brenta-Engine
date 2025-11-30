// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <demo/entities/sphere_entity.hpp>

#include <demo/components/model_component.hpp>
#include <demo/components/physics_component.hpp>
#include <demo/components/sphere_collider_component.hpp>
#include <demo/components/transform_component.hpp>

#include <brenta/engine.hpp>
#include <viotecs/viotecs.hpp>

#include <filesystem>

using namespace viotecs;
using namespace viotecs::types;
using namespace brenta;

void init_sphere_entity()
{
  /* ************************************************
   *
   *    Sphere 1
   *
   * ************************************************ */

  /* Create the sphere entity */
  auto sphere_entity1 = world::new_entity();

  /* Add the transform component */
  auto transform_component1 =
    TransformComponent(glm::vec3(2.0f, 1.0f, 0.0f), glm::vec3(0.0f), 1.0f);
  world::add_component<TransformComponent>(sphere_entity1,
                                           transform_component1);

  /* Add the physics component */
  auto physics_component1 =
    PhysicsComponent(1.0f,                        // mass
                     1.0f,                        // density
                     glm::vec3(0.0f, 0.0f, 0.0f), // velocity
                     glm::vec3(-1.f, 0.0f, 0.0f), // acceleration
                     true                         // isElastic
    );
  world::add_component<PhysicsComponent>(sphere_entity1, physics_component1);

  /* Add the sphere collider component */
  auto sphere_collider_component1 = SphereColliderComponent(1.0f);
  world::add_component<SphereColliderComponent>(sphere_entity1,
                                                sphere_collider_component1);

  /* Load the shader */
  if (shader::get_id("default_shader") == 0)
  {
    shader::create("default_shader", GL_VERTEX_SHADER,
                   std::filesystem::absolute("examples/demo/shaders/shader.vs"),
                   GL_FRAGMENT_SHADER,
                   std::filesystem::absolute("examples/demo/shaders/shader.fs"));
  }

  /* Load the model */
  model m1(std::filesystem::absolute("examples/assets/models/sphere/sphere.obj"));

  /* Add the model component */
  auto model_component1 = ModelComponent(m1, 32.0f, "default_shader");
  world::add_component<ModelComponent>(sphere_entity1,
                                       std::move(model_component1));

  /* ************************************************
   *
   *    Sphere 2
   *
   * ************************************************ */

  /* Create the sphere entity */
  auto sphere_entity2 = world::new_entity();

  /* Add the transform component */
  auto transform_component2 =
    TransformComponent(glm::vec3(-2.0f, 1.0f, 0.0f), glm::vec3(0.0f), 1.0f);
  world::add_component<TransformComponent>(sphere_entity2,
                                           transform_component2);

  /* Add the physics component */
  auto physics_component2 =
    PhysicsComponent(1.0f,                        // mass
                     1.0f,                        // density
                     glm::vec3(0.0f, 0.0f, 0.0f), // velocity
                     glm::vec3(1.0f, 0.0f, 0.0f), // acceleration
                     true                         // isElastic
    );
  world::add_component<PhysicsComponent>(sphere_entity2, physics_component2);

  /* Add the sphere collider component */
  auto sphere_collider_component2 = SphereColliderComponent(1.0f);
  world::add_component<SphereColliderComponent>(sphere_entity2,
                                                sphere_collider_component2);

  /* Load the shader */
  if (shader::get_id("default_shader") == 0)
  {
    shader::create("default_shader", GL_VERTEX_SHADER,
                   std::filesystem::absolute("examples/demo/shaders/shader.vs"),
                   GL_FRAGMENT_SHADER,
                   std::filesystem::absolute("examples/demo/shaders/shader.fs"));
  }

  /* Load the model */
  model m2(std::filesystem::absolute("examples/assets/models/sphere/sphere.obj"));

  /* Add the model component */
  auto model_component2 = ModelComponent(m2, 32.0f, "default_shader");
  world::add_component<ModelComponent>(sphere_entity2,
                                       std::move(model_component2));
}
