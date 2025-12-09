// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#include <brenta/error.hpp>

using namespace brenta;

std::vector<std::reference_wrapper<subsystem>> engine::subsystems;

std::expected<void, std::string> engine::initialize()
{
  for (auto& s : this->subsystems)
  {
    if (!s.get().initialize().has_value())
      return std::unexpected(s.get().subsystem_name);
  }

  INFO("engine initialized")
  return {};
}

std::expected<void, std::string> engine::terminate()
{
  for (auto it = this->subsystems.rbegin();
       it != this->subsystems.rend(); ++it) {
    auto& s = *it;

    if (!s.get().terminate().has_value())
      return std::unexpected(s.get().subsystem_name);

  }
  INFO("engine terminated");
  return {};
}

engine &engine::instance()
{
  static brenta::engine _instance;
  return _instance;
}

std::expected<void, std::string> engine::add_subsystem(subsystem::builder &&builder)
{
  std::reference_wrapper<brenta::subsystem> s = builder.build();
  if (!s.get().initialize().has_value())
    return std::unexpected(s.get().subsystem_name);
  engine::subsystems.push_back(s);
  return {};
}

//
// Builder
//

brenta::engine::builder &engine::builder::subsystem(subsystem::builder &builder)
{
  this->subsystems.push_back(builder.build());
  return *this;
}

brenta::engine::builder &engine::builder::subsystem(subsystem::builder &&builder)
{
  this->subsystems.push_back(builder.build());
  return *this;
}

subsystem &engine::builder::build()
{
  engine::subsystems = this->subsystems;
  return engine::instance();
}
