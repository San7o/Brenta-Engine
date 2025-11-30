// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <demo/entities/floor_entity.hpp>
#include <demo/components/model_component.hpp>
#include <demo/components/transform_component.hpp>

#include <brenta/engine.hpp>
#include <viotecs/viotecs.hpp>

#include <filesystem>

using namespace viotecs;
using namespace viotecs::types;
using namespace brenta;

void init_floor_entity()
{
  /* Create the floor entity */
  auto floor_entity = world::new_entity();

  /* Add the transform component */
  auto transform_component =
    TransformComponent(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), 1.0f);
  world::add_component<TransformComponent>(floor_entity, transform_component);

  if (shader::get_id("default_shader") == 0)
  {
    /* Load the shader */
    shader::create("default_shader", GL_VERTEX_SHADER,
                   std::filesystem::absolute("examples/demo/shaders/shader.vs"),
                   GL_FRAGMENT_SHADER,
                   std::filesystem::absolute("examples/demo/shaders/shader.fs"));
  }

  /* Load the model */
  model m(std::filesystem::absolute("examples/assets/models/pane/pane.obj"));

  /* Add the model component */
  auto model_component = ModelComponent(m, 32.0f, "default_shader");
  world::add_component<ModelComponent>(floor_entity,
                                       std::move(model_component));
}
