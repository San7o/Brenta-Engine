// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/asset_manager.hpp>
#include <brenta/renderer/model.hpp>
#include <demo/components/model.hpp>
#include <demo/components/player.hpp>
#include <demo/components/transform.hpp>
#include <demo/entities/player.hpp>

#include <viotecs/viotecs.hpp>

using namespace viotecs;
using namespace brenta;

#include "../../../../src/renderer/shaders/c/phong_vs.c"
#include "../../../../src/renderer/shaders/c/phong_fs.c"

void init_player_entity()
{
  auto shader = AssetManager::get_shader("default_shader");
  if (!shader)
  {
    shader = AssetManager::new_shader("default_shader", {
        { Shader::Type::Vertex,   phong_vs },
        { Shader::Type::Fragment, phong_fs } });
  }

  Model m = Model::Builder()
    .path("examples/assets/models/backpack/backpack.obj")
    .transform(Transform()
               .translate(glm::vec3(0.0f, 1.8f, -5.0f))
               .scale(glm::vec3(1.0f)))
    .build();
  
  auto player = World::new_entity()
    .add_component<PlayerComponent>()
    .add_component<TransformComponent>(m.get_transform())
    .add_component<ModelComponent>(std::move(m), 32.0f, shader);
}
