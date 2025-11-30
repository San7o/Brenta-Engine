// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <demo/components/model_component.hpp>
#include <demo/components/transform_component.hpp>
#include <demo/entities/robot_entity.hpp>

#include <brenta/engine.hpp>
#include <viotecs/viotecs.hpp>

#include <filesystem>

using namespace brenta;
using namespace viotecs;

void init_robot_entity()
{
  /* Create the cube entity */
  auto cube_entity = world::new_entity();

  /* Add the transform component */
  auto transform_component = TransformComponent(
    glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, -90.0f, 0.0f), 5.0f);
  world::add_component<TransformComponent>(cube_entity, transform_component);

  /* Load the shader */
  if (shader::get_id("default_shader") == 0)
  {
    shader::create("default_shader", GL_VERTEX_SHADER,
                   std::filesystem::absolute("examples/demo/shaders/shader.vs"),
                   GL_FRAGMENT_SHADER,
                   std::filesystem::absolute("examples/demo/shader.fs"));
  }

  /* Load the model */
  model m(
    std::filesystem::absolute("examples/assets/models/robot_sprite/robot_sprite.obj"),
    GL_REPEAT, GL_NEAREST, GL_NEAREST, GL_TRUE, GL_LINEAR_MIPMAP_NEAREST,
    GL_NEAREST, false);

  /* Add the model component */
  auto model_component = ModelComponent(m, 32.0f, "default_shader", true, 4, 0);

  world::add_component<ModelComponent>(cube_entity, std::move(model_component));
}
