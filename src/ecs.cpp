// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/ecs.hpp>
#include <brenta/logger.hpp>

using namespace brenta;

//
// Static variables
//

const std::string ecs::subsystem_name = "ecs";

//
// Subsystem interface
//

std::expected<void, subsystem::error> ecs::initialize()
{
  viotecs::world::init();

  INFO("{}: initialized", ecs::subsystem_name);
  return {};
}

std::expected<void, subsystem::error> ecs::terminate()
{
  viotecs::world::destroy();

  INFO("{}: terminated", ecs::subsystem_name);
  return {};
}

std::string ecs::name()
{
  return ecs::subsystem_name;
}

//
// Member functions
//

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
