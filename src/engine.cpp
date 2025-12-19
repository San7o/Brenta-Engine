// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>

#include <stdexcept>

using namespace brenta;

//
// Static variables
//

std::vector<std::reference_wrapper<subsystem>> engine::subsystems;
const std::string engine::subsystem_name = "engine";
bool engine::initialized = false;

//
// Subsystem interface
//

std::expected<void, subsystem::error> engine::initialize()
{
  if (this->is_initialized()) return {};
  
  for (auto& s : this->subsystems)
  {
    auto ret = s.get().initialize();
    if (!ret.has_value())
      return std::unexpected(engine::subsystem_name + ": failed to initialize "
                             + s.get().name() + ": " + ret.error());
  }

  engine::initialized = true;
  INFO("{}: initialized", engine::subsystem_name);
  return {};
}

std::expected<void, subsystem::error> engine::terminate()
{
  if (!this->is_initialized()) return {};
  
  for (auto it = this->subsystems.rbegin();
       it != this->subsystems.rend(); ++it)
  {
    auto& s = *it;
    
    auto ret = s.get().terminate();
    if (!ret.has_value())
      return std::unexpected(engine::subsystem_name + "failed to terminate "
                             + s.get().name() + ": " + ret.error());
  }

  engine::initialized = false;
  INFO("{}: terminated", engine::subsystem_name);
  return {};
}

std::string engine::name()
{
  return engine::subsystem_name;
}

bool engine::is_initialized()
{
  return engine::initialized;
}

//
// Member functions
//

engine &engine::instance()
{
  static brenta::engine _instance;
  return _instance;
}

engine::manager engine::managed()
{
  return engine::manager();
}

std::expected<void, std::string>
engine::with(subsystem::builder &&builder)
{
  std::reference_wrapper<brenta::subsystem> s = builder.build();
  if (!s.get().initialize().has_value())
    return std::unexpected(s.get().name());
  engine::subsystems.push_back(s);
  return {};
}

engine::manager::manager()
{
  auto ret = engine::instance().initialize();
  if (!ret.has_value())
  {
    ERROR("engine::manager: failed to initialize subsystem, {}", ret.error());
    throw std::runtime_error("engine::manager: failed to initialize subsystem,"
                             + ret.error());
  }
}

engine::manager::~manager()
{
  auto ret = engine::instance().terminate();
  if (!ret.has_value())
  {
    ERROR("engine::manager: failed to terminate subsystem, {}", ret.error());
  }
}

//
// Builder
//

brenta::engine::builder&
engine::builder::with(subsystem::builder &builder)
{
  this->subsystems.push_back(builder.build());
  return *this;
}

brenta::engine::builder&
engine::builder::with(subsystem::builder &&builder)
{
  this->subsystems.push_back(builder.build());
  return *this;
}

subsystem &engine::builder::build()
{
  engine::subsystems = this->subsystems;
  return engine::instance();
}
