// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/opengl/shader.hpp>
#include <brenta/renderer/model.hpp>
#include <demo/components/model.hpp>
#include <demo/components/physics.hpp>
#include <demo/components/sphere_collider.hpp>
#include <demo/components/transform.hpp>
#include <demo/entities/sphere.hpp>

#include <viotecs/viotecs.hpp>

using namespace viotecs;
using namespace viotecs::types;
using namespace brenta;

void init_sphere_entity()
{
  //
  // Spere 1
  //

  auto shader = Shader::get_shader("default_shader");
  if (!shader)
  {
    Shader::create("default_shader",
                   Shader::Type::Vertex,   "examples/demo/shaders/shader.vs",
                   Shader::Type::Fragment, "examples/demo/shaders/shader.fs");
  }
  Model m1 = Model::Builder()
    .path("examples/assets/models/sphere/sphere.obj")
    .build();

  auto sphere1 = world::new_entity()
    .add_component<TransformComponent>(glm::vec3(2.0f, 1.0f, 0.0f),
                                       glm::vec3(0.0f), 1.0f)
    .add_component<PhysicsComponent>(1.0f,                        // mass
                                     1.0f,                        // density
                                     glm::vec3(0.0f, 0.0f, 0.0f), // velocity
                                     glm::vec3(-1.f, 0.0f, 0.0f), // acceleration
                                     true)                        // isElastic
    .add_component<SphereColliderComponent>(1.0f)
    .add_component<ModelComponent>(std::move(m1), 32.0f, "default_shader");

  //
  // Spehere 2
  //

  Model m2 = Model::Builder()
    .path("examples/assets/models/sphere/sphere.obj")
    .build();
  
  auto sphere2 = world::new_entity()
    .add_component<TransformComponent>(glm::vec3(-2.0f, 1.0f, 0.0f),
                                       glm::vec3(0.0f), 1.0f)
    .add_component<PhysicsComponent>(1.0f,                        // mass
                                     1.0f,                        // density
                                     glm::vec3(0.0f, 0.0f, 0.0f), // velocity
                                     glm::vec3(1.0f, 0.0f, 0.0f), // acceleration
                                     true)                        // isElastic
    .add_component<SphereColliderComponent>(1.0f)
    .add_component<ModelComponent>(std::move(m2), 32.0f, "default_shader");
}
