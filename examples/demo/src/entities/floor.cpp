// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/asset_manager.hpp>
#include <brenta/renderer/model.hpp>

#include <demo/components/model.hpp>
#include <demo/components/transform.hpp>
#include <demo/entities/floor.hpp>

#include <viotecs/viotecs.hpp>
#include <tenno/utility.hpp>

#include "../../../../src/renderer/shaders/c/phong_vs.c"
#include "../../../../src/renderer/shaders/c/phong_fs.c"

using namespace viotecs;
using namespace brenta;

void init_floor_entity()
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
    .path("examples/assets/models/pane/pane.obj")
    .build();

  auto floor = World::new_entity()
    .add_component<TransformComponent>(m.get_transform())
    .add_component<ModelComponent>(tenno::move(m), 32.0f, shader);
}
