// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/opengl/shader.hpp>
#include <brenta/renderer/model.hpp>
#include <demo/components/model.hpp>
#include <demo/components/transform.hpp>
#include <demo/entities/cube.hpp>

#include <viotecs/viotecs.hpp>

using namespace viotecs;
using namespace viotecs::types;
using namespace brenta;

void init_cube_entity()
{
  if (Shader::get_id("cube_shader") == 0)
  {
    Shader::create("cube_shader",
                   Shader::Type::Vertex,   "examples/demo/shaders/shader.vs",
                   Shader::Type::Fragment, "examples/demo/shaders/shader.fs");
  }

  Model m = Model::Builder()
    .path("examples/assets/models/simple_cube/simple_cube.obj")
    .build();

  auto cube = world::new_entity()
    .add_component<TransformComponent>(glm::vec3(0.0f), glm::vec3(0.0f), 1.0f)
    .add_component<ModelComponent>(std::move(m), 32.0f, "cube_shader");
}
