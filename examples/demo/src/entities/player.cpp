// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/opengl/shader.hpp>
#include <brenta/renderer/model.hpp>
#include <demo/components/model.hpp>
#include <demo/components/player.hpp>
#include <demo/components/transform.hpp>
#include <demo/entities/player.hpp>

#include <viotecs/viotecs.hpp>

using namespace viotecs;
using namespace viotecs::types;
using namespace brenta;

void init_player_entity()
{
  if (shader::get_id("default_shader") == 0)
  {
    shader::create("default_shader",
                   shader::type::vertex, "examples/demo/shaders/shader.vs",
                   shader::type::fragment, "examples/demo/shaders/shader.fs");
  }

  model m = model::builder()
    .path("examples/assets/models/backpack/backpack.obj")
    .build();

  auto player = world::new_entity()
    .add_component<PlayerComponent>()
    .add_component<TransformComponent>(glm::vec3(0.0f, 1.8f, -5.0f),
                                       glm::vec3(0.0f), 1.0f)
    .add_component<ModelComponent>(std::move(m), 32.0f, "default_shader");
}
