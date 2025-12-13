// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#include <demo/components/model_component.hpp>
#include <demo/components/physics_component.hpp>
#include <demo/components/sphere_collider_component.hpp>
#include <demo/components/transform_component.hpp>
#include <demo/entities/sphere_entity.hpp>
#include <viotecs/viotecs.hpp>

using namespace viotecs;
using namespace viotecs::types;
using namespace brenta;

void init_sphere_entity()
{
  //
  // Spere 1
  //
  
  if (shader::get_id("default_shader") == 0)
  {
    shader::create("default_shader",
      GL_VERTEX_SHADER, "examples/demo/shaders/shader.vs",
      GL_FRAGMENT_SHADER, "examples/demo/shaders/shader.fs");
  }
  model m1("examples/assets/models/sphere/sphere.obj");

  auto sphere1 = world::new_entity()
    .add_component<TransformComponent>(glm::vec3(2.0f, 1.0f, 0.0f),
                                       glm::vec3(0.0f), 1.0f)
    .add_component<PhysicsComponent>(1.0f,                        // mass
                                     1.0f,                        // density
                                     glm::vec3(0.0f, 0.0f, 0.0f), // velocity
                                     glm::vec3(-1.f, 0.0f, 0.0f), // acceleration
                                     true)                        // isElastic
    .add_component<SphereColliderComponent>(1.0f)
    .add_component<ModelComponent>(m1, 32.0f, "default_shader");

  //
  // Spehere 2
  //
  
  if (shader::get_id("default_shader") == 0)
  {
    shader::create("default_shader",
      GL_VERTEX_SHADER, "examples/demo/shaders/shader.vs",
      GL_FRAGMENT_SHADER, "examples/demo/shaders/shader.fs");
  }

  model m2("examples/assets/models/sphere/sphere.obj");
  
  auto sphere2 = world::new_entity()
    .add_component<TransformComponent>(glm::vec3(-2.0f, 1.0f, 0.0f),
                                       glm::vec3(0.0f), 1.0f)
    .add_component<PhysicsComponent>(1.0f,                        // mass
                                     1.0f,                        // density
                                     glm::vec3(0.0f, 0.0f, 0.0f), // velocity
                                     glm::vec3(1.0f, 0.0f, 0.0f), // acceleration
                                     true)                        // isElastic
    .add_component<SphereColliderComponent>(1.0f)
    .add_component<ModelComponent>(m2, 32.0f, "default_shader");
}
