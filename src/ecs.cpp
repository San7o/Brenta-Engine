// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/ecs.hpp>
#include <brenta/logger.hpp>

using namespace brenta;

std::expected<void, std::string> ecs::initialize()
{
  viotecs::world::init();

  INFO("ecs initialized");
  return {};
}

std::expected<void, std::string> ecs::terminate()
{
  viotecs::world::destroy();

  INFO("ecs terminated");
  return {};
}

ecs &ecs::instance()
{
  static ecs _ecs;
  return _ecs;
}

//
// Builder
//

subsystem &ecs::builder::build()
{
  return ecs::instance();
}
