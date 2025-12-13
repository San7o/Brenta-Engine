// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <demo/components/model.hpp>
#include <demo/components/transform.hpp>
#include <demo/entities/floor.hpp>
#include <brenta/engine.hpp>

#include <viotecs/viotecs.hpp>

using namespace viotecs;
using namespace viotecs::types;
using namespace brenta;

void init_floor_entity()
{
  if (shader::get_id("default_shader") == 0)
  {
    shader::create("default_shader",
      GL_VERTEX_SHADER, "examples/demo/shaders/shader.vs",
      GL_FRAGMENT_SHADER, "examples/demo/shaders/shader.fs");
  }

  model m("examples/assets/models/pane/pane.obj");

  auto floor = world::new_entity()
    .add_component<TransformComponent>(glm::vec3(0.0f, 0.0f, 0.0f),
                                       glm::vec3(0.0f), 1.0f)
    .add_component<ModelComponent>(m, 32.0f, "default_shader");
}
