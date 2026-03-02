// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/asset_manager.hpp>
#include <brenta/renderer/model.hpp>
#include <brenta/ecs/ecs.hpp>
#include <brenta/ecs/components/model_ecs_component.hpp>
#include <brenta/ecs/components/transform_ecs_component.hpp>
#include <brenta/logger.hpp>

#include <demo/components/sprite_animation.hpp>
#include <demo/entities.hpp>
#include <demo/utils.hpp>

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
    auto shader_builder =
      Shader::Builder()
      .objects({
          { Shader::Type::Vertex,   phong_vs },
          { Shader::Type::Fragment, phong_fs } });
    shader = AssetManager::new_asset<Shader>("default_shader",
                                             shader_builder);
  }
  if (!shader)
    return;
  
  auto robot_material_builder =
    Material::Builder()
    .shader(shader)
    .floating("material.shininess", 32.0f)
    .integer("atlasIndex", 0);
  auto robot_material =
    AssetManager::new_asset<Material>("robot_material",
                                      robot_material_builder);

  auto model_builder =
    Model::Builder()
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
                   .mipmap_mag(Texture::Filtering::Nearest))
    .material(robot_material);

  auto cube_entity = World::new_entity()
    .add_component<TransformEcsComponent>()
    .add_component<SpriteAnimationEcsComponent>(4, 0, 60)
    .add_component<ModelEcsComponent>(model_builder);
}
