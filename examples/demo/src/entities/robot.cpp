// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/opengl/shader.hpp>
#include <brenta/renderer/model.hpp>
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
                   shader::type::vertex, "examples/demo/shaders/shader.vs",
                   shader::type::fragment, "examples/demo/shader.fs");
  }

  model m = model::builder()
    .path("examples/assets/models/robot_sprite/robot_sprite.obj")
    .wrapping(GL_REPEAT)
    .filtering_min(GL_NEAREST)
    .filtering_mag(GL_NEAREST)
    .has_mipmap(GL_TRUE)
    .mipmap_min(GL_LINEAR_MIPMAP_NEAREST)
    .mipmap_mag(GL_NEAREST)
    .flip(false)
    .build();

  auto cube_entity = world::new_entity()
    .add_component<TransformComponent>(glm::vec3(0.0f, 5.0f, 0.0f),
                                       glm::vec3(0.0f, -90.0f, 0.0f), 5.0f)
    .add_component<ModelComponent>(std::move(m), 32.0f, "default_shader", true, 4, 0);
}
