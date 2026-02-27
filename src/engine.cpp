// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#include <brenta/logger.hpp>

#include <stdexcept>

using namespace brenta;

//
// Static variables
//

tenno::vector<tenno::reference_wrapper<Subsystem>> Engine::subsystems;
const std::string Engine::subsystem_name = "engine";
bool Engine::initialized = false;

//
// Subsystem interface
//

std::expected<void, Subsystem::Error> Engine::initialize()
{
  if (this->is_initialized()) return {};
  
  for (auto& s : this->subsystems)
  {
    auto ret = s.get().initialize();
    if (!ret.has_value())
      return std::unexpected(Engine::subsystem_name + ": failed to initialize "
                             + s.get().name() + ": " + ret.error());
  }

  Engine::initialized = true;
  INFO("{}: initialized", Engine::subsystem_name);
  return {};
}

std::expected<void, Subsystem::Error> Engine::terminate()
{
  if (!this->is_initialized()) return {};
  
  for (auto it = this->subsystems.rbegin();
       it != this->subsystems.rend(); ++it)
  {
    auto& s = *it;
    
    auto ret = s.get().terminate();
    if (!ret.has_value())
      return std::unexpected(Engine::subsystem_name + "failed to terminate "
                             + s.get().name() + ": " + ret.error());
  }

  Engine::initialized = false;
  INFO("{}: terminated", Engine::subsystem_name);
  return {};
}

std::string Engine::name()
{
  return Engine::subsystem_name;
}

bool Engine::is_initialized()
{
  return Engine::initialized;
}

//
// Member functions
//

Engine &Engine::instance()
{
  static brenta::Engine _instance;
  return _instance;
}

Engine::Manager Engine::managed()
{
  return Engine::Manager();
}

std::expected<void, std::string>
Engine::with(Subsystem::Builder &&builder)
{
  tenno::reference_wrapper<brenta::Subsystem> s = builder.build();
  if (!s.get().initialize().has_value())
    return std::unexpected(s.get().name());
  Engine::subsystems.push_back(s);
  return {};
}

Engine::Manager::Manager()
{
  auto ret = Engine::instance().initialize();
  if (!ret.has_value())
  {
    ERROR("Engine::Manager: failed to initialize subsystem, {}", ret.error());
    throw std::runtime_error("Engine::Manager: failed to initialize subsystem,"
                             + ret.error());
  }
  return;
}

Engine::Manager::~Manager()
{
  auto ret = Engine::instance().terminate();
  if (!ret.has_value())
  {
    ERROR("Engine::Manager: failed to terminate subsystem, {}", ret.error());
  }
  return;
}

//
// Builder
//

brenta::Engine::Builder&
Engine::Builder::with(Subsystem::Builder &builder)
{
  this->subsystems.push_back(builder.build());
  return *this;
}

brenta::Engine::Builder&
Engine::Builder::with(Subsystem::Builder &&builder)
{
  this->subsystems.push_back(builder.build());
  return *this;
}

Subsystem &Engine::Builder::build()
{
  Engine::subsystems = this->subsystems;
  return Engine::instance();
}
