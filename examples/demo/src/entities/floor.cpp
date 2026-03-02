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

#include <demo/entities.hpp>
#include <demo/utils.hpp>

#include <tenno/utility.hpp>

using namespace viotecs;
using namespace brenta;

void init_floor_entity()
{
  auto default_material = get_default_material();
  if (!default_material)
  {
    ERROR("Demo: error getting default material");
    return;
  }
  
  auto model_builder =
    Model::Builder()
    .path("examples/assets/models/pane/pane.obj")
    .material(default_material);

  auto floor = World::new_entity()
    .add_component<TransformEcsComponent>()
    .add_component<ModelEcsComponent>(model_builder);

  return;
}
