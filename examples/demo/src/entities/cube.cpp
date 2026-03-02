// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/asset_manager.hpp>
#include <brenta/transform.hpp>
#include <brenta/renderer/model.hpp>
#include <brenta/ecs/ecs.hpp>
#include <brenta/ecs/components/model_ecs_component.hpp>
#include <brenta/ecs/components/transform_ecs_component.hpp>
#include <brenta/logger.hpp>

#include <demo/entities.hpp>
#include <demo/utils.hpp>

#include <tenno/utility.hpp>

using namespace viotecs;
using namespace brenta;

void init_cube_entity()
{
  auto default_material = get_default_material();
  if (!default_material)
  {
    ERROR("Demo: error getting default material");
    return;
  }

  auto transform =
    Transform()
    .translate({3.0, 1.0, 5.0});
  
  auto model_builder =
    Model::Builder()
    .path("examples/assets/models/simple_cube/simple_cube.obj")
    .material(default_material);
  auto model =
    AssetManager::new_asset<Model>("cube", model_builder);
  auto cube =
    World::new_entity()
    .add_component<TransformEcsComponent>(transform)
    .add_component<ModelEcsComponent>(model);

  return;
}
