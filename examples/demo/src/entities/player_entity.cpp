// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#include <demo/components/model_component.hpp>
#include <demo/components/player_component.hpp>
#include <demo/components/transform_component.hpp>
#include <demo/entities/player_entity.hpp>
#include <filesystem>
#include <viotecs/viotecs.hpp>

using namespace viotecs;
using namespace viotecs::types;
using namespace brenta;

void init_player_entity()
{
  /* Create the player entity */
  auto player_entity = world::new_entity();

  /* Add the player component */
  world::add_component<PlayerComponent>(player_entity, PlayerComponent());

  /* Add the transform component */
  auto transform_component =
    TransformComponent(glm::vec3(0.0f, 1.8f, -5.0f), glm::vec3(0.0f), 1.0f);
  world::add_component<TransformComponent>(player_entity, transform_component);

  /* Load the shader */
  if (shader::get_id("default_shader") == 0)
  {
    shader::create(
      "default_shader", GL_VERTEX_SHADER,
      std::filesystem::absolute("examples/demo/shaders/shader.vs"),
      GL_FRAGMENT_SHADER,
      std::filesystem::absolute("examples/demo/shaders/shader.fs"));
  }

  /* Load the model */
  model m(
    std::filesystem::absolute("examples/assets/models/backpack/backpack.obj"));

  /* Add the model component */
  auto model_component = ModelComponent(m, 32.0f, "default_shader");
  world::add_component<ModelComponent>(player_entity,
                                       std::move(model_component));
}
