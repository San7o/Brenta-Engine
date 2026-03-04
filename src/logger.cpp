// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/logger.hpp>

using namespace brenta;

//
// Static variables
//

const std::string Logger::subsystem_name = "logger";
Logger::Config Logger::init_config       = {};
bool Logger::initialized                 = false;

//
// Subsystem interface
//

std::string Logger::event_name(enum Event event)
{
  switch(event)
  {
  case Logger::Event::Lifetime: return "lifetime";
  default:                      return "unknown";
  }
}

std::expected<void, Subsystem::Error> Logger::initialize()
{
  if (this->is_initialized()) return {};
  
  oak::set_level(Logger::init_config.log_level);

  for (auto event : Logger::init_config.events)
    oak::enable_event(event, Logger::event_name(event));

  if (Logger::init_config.flags.size() == 0)
  {
    // Default
    oak::set_flags(oak::Flags::Level);
  }
  else
  {
    oak::set_flags(oak::Flags::None);
    for (auto& flag : Logger::init_config.flags)
      oak::add_flags(flag);
  }

  auto file_name = Logger::init_config.log_file;
  if (file_name != "")
  {
    oak::add_writer<oak::FileWriter>(file_name);
    INFO("{}: set logging to file to {}", Logger::subsystem_name, file_name.string());
  }

  Logger::initialized = true;
  INFO("{}: initialized", Logger::subsystem_name);
  return {};
}

std::expected<void, Subsystem::Error> Logger::terminate()
{
  if (!this->is_initialized()) return {};
  
  Logger::initialized = false;
  return {};
}

std::string Logger::name()
{
  return Logger::subsystem_name;
}

bool Logger::is_initialized()
{
  return Logger::initialized;
}

//
// Member functions
//

Logger &Logger::instance()
{
  static Logger _logger;
  return _logger;
}

//
// Builder
//

Logger::Builder &Logger::Builder::level(Logger::Level log_level)
{
  this->conf.log_level = log_level;
  return *this;
}

Logger::Builder &Logger::Builder::file(std::filesystem::path log_file)
{
  this->conf.log_file = log_file;
  return *this;
}

Logger::Builder &Logger::Builder::event(Logger::Event event)
{
  this->conf.events.push_back(event);
  return *this;
}

Logger::Builder &Logger::Builder::flag(Logger::Flags flag)
{
  this->conf.flags.push_back(flag);
  return *this;
}

Subsystem &Logger::Builder::build()
{
  Logger::init_config = this->conf;
  return Logger::instance();
}
