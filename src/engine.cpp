// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>

using namespace brenta;

//
// Static variables
//

std::vector<std::reference_wrapper<subsystem>> engine::subsystems;
const std::string engine::subsystem_name = "engine";

//
// Subsystem interface
//

std::expected<void, subsystem::error> engine::initialize()
{
  for (auto& s : this->subsystems)
  {
    auto ret = s.get().initialize();
    if (!ret.has_value())
      return std::unexpected(engine::subsystem_name + ": failed to initialize"
                             + s.get().name() + ": " + ret.error());
  }

  INFO("{}: initialized", engine::subsystem_name);
  return {};
}

std::expected<void, subsystem::error> engine::terminate()
{
  for (auto it = this->subsystems.rbegin();
       it != this->subsystems.rend(); ++it)
  {
    auto& s = *it;
    
    auto ret = s.get().terminate();
    if (!ret.has_value())
      return std::unexpected(engine::subsystem_name + "failed to terminate"
                             + s.get().name() + ": " + ret.error());
  }
  
  INFO("{}: engine terminated", engine::subsystem_name);
  return {};
}

std::string engine::name()
{
  return engine::subsystem_name;
}

//
// Member functions
//

engine &engine::instance()
{
  static brenta::engine _instance;
  return _instance;
}

std::expected<void, std::string>
engine::add_subsystem(subsystem::builder &&builder)
{
  std::reference_wrapper<brenta::subsystem> s = builder.build();
  if (!s.get().initialize().has_value())
    return std::unexpected(s.get().name());
  engine::subsystems.push_back(s);
  return {};
}

//
// Builder
//

brenta::engine::builder&
engine::builder::subsystem(subsystem::builder &builder)
{
  this->subsystems.push_back(builder.build());
  return *this;
}

brenta::engine::builder&
engine::builder::subsystem(subsystem::builder &&builder)
{
  this->subsystems.push_back(builder.build());
  return *this;
}

subsystem &engine::builder::build()
{
  engine::subsystems = this->subsystems;
  return engine::instance();
}
