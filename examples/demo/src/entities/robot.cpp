// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#include <demo/components/model.hpp>
#include <demo/components/transform.hpp>
#include <demo/entities/robot.hpp>

#include <viotecs/viotecs.hpp>

using namespace brenta;
using namespace viotecs;

void init_robot_entity()
{
  if (shader::get_id("default_shader") == 0)
  {
    shader::create("default_shader",
                   GL_VERTEX_SHADER, "examples/demo/shaders/shader.vs",
                   GL_FRAGMENT_SHADER,"examples/demo/shader.fs");
  }

  model m("examples/assets/models/robot_sprite/robot_sprite.obj",
          GL_REPEAT, GL_NEAREST, GL_NEAREST, GL_TRUE, GL_LINEAR_MIPMAP_NEAREST,
          GL_NEAREST, false);

  auto cube_entity = world::new_entity()
    .add_component<TransformComponent>(glm::vec3(0.0f, 5.0f, 0.0f),
                                       glm::vec3(0.0f, -90.0f, 0.0f), 5.0f)
    .add_component<ModelComponent>(m, 32.0f, "default_shader", true, 4, 0);
}
