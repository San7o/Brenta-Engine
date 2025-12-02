// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#include <demo/components/model_component.hpp>
#include <demo/components/transform_component.hpp>
#include <demo/entities/cube_entity.hpp>
#include <filesystem>
#include <viotecs/viotecs.hpp>

using namespace viotecs;
using namespace viotecs::types;
using namespace brenta;

void init_cube_entity()
{
  /* Create the cube entity */
  auto cube_entity = world::new_entity();

  /* Add the transform component */
  auto transform_component =
    TransformComponent(glm::vec3(0.0f), glm::vec3(0.0f), 1.0f);
  world::add_component<TransformComponent>(cube_entity, transform_component);

  /* Load the shader */
  if (shader::get_id("cube_shader") == 0)
  {
    shader::create(
      "cube_shader", GL_VERTEX_SHADER,
      std::filesystem::absolute("examples/demo/shaders/shader.vs"),
      GL_FRAGMENT_SHADER,
      std::filesystem::absolute("examples/demo/shaders/shader.fs"));
  }

  /* Load the model */
  model m(std::filesystem::absolute(
    "examples/assets/models/simple_cube/simple_cube.obj"));

  /* Add the model component */
  auto model_component = ModelComponent(m, 32.0f, "cube_shader");
  world::add_component<ModelComponent>(cube_entity, std::move(model_component));
}
