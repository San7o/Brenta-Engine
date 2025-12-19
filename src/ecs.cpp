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
bool ecs::initialized = false;

//
// Subsystem interface
//

std::expected<void, subsystem::error> ecs::initialize()
{
  if (this->is_initialized()) return {};
  
  viotecs::world::init();

  ecs::initialized = true;
  INFO("{}: initialized", ecs::subsystem_name);
  return {};
}

std::expected<void, subsystem::error> ecs::terminate()
{
  if (!this->is_initialized()) return {};

  viotecs::world::destroy();

  ecs::initialized = true;
  INFO("{}: terminated", ecs::subsystem_name);
  return {};
}

std::string ecs::name()
{
  return ecs::subsystem_name;
}

bool ecs::is_initialized()
{
  return ecs::initialized;
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
