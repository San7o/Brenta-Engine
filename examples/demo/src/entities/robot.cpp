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

#include "../../../assets/shaders/c/default_shader_vs.c"
#include "../../../assets/shaders/c/default_shader_fs.c"

void init_robot_entity()
{
  auto shader = Shader::get_shader("default_shader");
  if (!shader)
  {
    Shader::create("default_shader", {
        { Shader::Type::Vertex, default_shader_vs },
        { Shader::Type::Fragment, default_shader_fs } });
  }

  Model m = Model::Builder()
    .path("examples/assets/models/robot_sprite/robot_sprite.obj")
    .transform(Transform()
               .translate(glm::vec3(0.0f, 5.0f, 0.0f))
               .rotate(glm::angleAxis(glm::radians(-90.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f)))
               .scale(glm::vec3(5.0f)))
    .wrapping(Texture::Wrapping::Repeat)
    .filtering_min(Texture::Filtering::Nearest)
    .filtering_mag(Texture::Filtering::Nearest)
    .has_mipmap(Gl::True)
    .mipmap_min(Texture::Filtering::LinearMipmapNearest)
    .mipmap_mag(Texture::Filtering::Nearest)
    .flip(false)
    .build();

  auto cube_entity = world::new_entity()
    .add_component<TransformComponent>(m.get_transform())
    .add_component<ModelComponent>(std::move(m), 32.0f, "default_shader", true, 4, 0);
}
