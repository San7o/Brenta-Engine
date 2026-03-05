// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/asset.hpp>
#include <demo/utils.hpp>

#include "../../../../src/renderer/shaders/c/phong_vs.c"
#include "../../../../src/renderer/shaders/c/phong_fs.c"

using namespace brenta;

tenno::shared_ptr<Material> get_default_material()
{
  auto material = AssetManager::get<Material>("default_material");
  if (!material)
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
      return nullptr;

    auto material_builder =
      Material::Builder()
      .shader(shader)
      .integer("atlasIndex", 0)
      .floating("material.shininess", 32.0f);
    material = AssetManager::new_asset<Material>("default_material",
                                                 material_builder);
  }
  return material;
}
