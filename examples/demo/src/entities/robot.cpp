// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/asset_manager.hpp>
#include <brenta/renderer/model.hpp>

#include <demo/components/model.hpp>
#include <demo/components/transform.hpp>
#include <demo/entities/robot.hpp>

#include <viotecs/viotecs.hpp>
#include <tenno/utility.hpp>

#include "../../../../src/renderer/shaders/c/phong_vs.c"
#include "../../../../src/renderer/shaders/c/phong_fs.c"

using namespace brenta;
using namespace viotecs;

void init_robot_entity()
{
  auto shader = AssetManager::get<Shader>("default_shader");
  if (!shader)
  {
    shader = AssetManager::new_asset<Shader>("default_shader",
                                             Shader::Builder()
                                             .objects({
                                                 { Shader::Type::Vertex,   phong_vs },
                                                 { Shader::Type::Fragment, phong_fs } }));
  }

  Model m = Model::Builder()
    .path("examples/assets/models/robot_sprite/robot_sprite.obj")
    .transform(Transform()
               .translate(glm::vec3(0.0f, 5.0f, 0.0f))
               .rotate(glm::angleAxis(glm::radians(-90.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f)))
               .scale(glm::vec3(5.0f)))
    .texture_props(Texture::Properties()
                   .wrapping(Texture::Wrapping::Repeat)
                   .filtering_min(Texture::Filtering::Nearest)
                   .filtering_mag(Texture::Filtering::Nearest)
                   .has_mipmap(Gl::True)
                   .mipmap_min(Texture::Filtering::LinearMipmapNearest)
                   .mipmap_mag(Texture::Filtering::Nearest)
                   .flipped(false))
    .build();

  auto cube_entity = World::new_entity()
    .add_component<TransformComponent>(m.get_transform())
    .add_component<ModelComponent>(tenno::move(m), 32.0f, shader, true, 4, 0);
}
