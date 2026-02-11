// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <demo/components/model.hpp>
#include <demo/components/transform.hpp>
#include <demo/entities/floor.hpp>
#include <brenta/renderer/opengl/shader.hpp>
#include <brenta/renderer/model.hpp>

#include <viotecs/viotecs.hpp>

using namespace viotecs;
using namespace viotecs::types;
using namespace brenta;

void init_floor_entity()
{
  if (Shader::get_id("default_shader") == 0)
  {
    Shader::create("default_shader",
                   Shader::Type::Vertex, "examples/demo/shaders/shader.vs",
                   Shader::Type::Fragment, "examples/demo/shaders/shader.fs");
  }

  Model m = Model::Builder()
    .path("examples/assets/models/pane/pane.obj")
    .build();

  auto floor = world::new_entity()
    .add_component<TransformComponent>(glm::vec3(0.0f, 0.0f, 0.0f),
                                       glm::vec3(0.0f), 1.0f)
    .add_component<ModelComponent>(std::move(m), 32.0f, "default_shader");
}
