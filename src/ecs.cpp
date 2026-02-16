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

const std::string Ecs::subsystem_name = "ecs";
bool Ecs::initialized = false;

//
// Subsystem interface
//

std::expected<void, Subsystem::Error> Ecs::initialize()
{
  if (this->is_initialized()) return {};
  
  viotecs::World::init();

  Ecs::initialized = true;
  INFO("{}: initialized", Ecs::subsystem_name);
  return {};
}

std::expected<void, Subsystem::Error> Ecs::terminate()
{
  if (!this->is_initialized()) return {};

  viotecs::World::destroy();

  Ecs::initialized = true;
  INFO("{}: terminated", Ecs::subsystem_name);
  return {};
}

std::string Ecs::name()
{
  return Ecs::subsystem_name;
}

bool Ecs::is_initialized()
{
  return Ecs::initialized;
}

//
// Member functions
//

Ecs &Ecs::instance()
{
  static Ecs _ecs;
  return _ecs;
}

//
// Builder
//

Subsystem &Ecs::Builder::build()
{
  return Ecs::instance();
}
