// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/logger.hpp>

using namespace brenta;

//
// Static variables
//

oak::level logger::log_level;
std::string logger::log_file;
const std::string logger::subsystem_name = "logger";

//
// Subsystem interface
//

std::expected<void, subsystem::error> logger::initialize()
{
  oak::init_writer();
  
  oak::set_level(this->log_level);
  
  if (this->log_file != "")
  {
    auto file = oak::set_file(this->log_file);
    if (!file.has_value())
    {
      ERROR("Failed to open log file: {}", log_file);
      return std::unexpected(this->subsystem_name);
    }
    INFO("set log file to {}", log_file);
  }

  INFO("logger initialized");
  return {};
}

std::expected<void, subsystem::error> logger::terminate()
{
  oak::stop_writer();
  return {};
}

std::string logger::name()
{
  return logger::subsystem_name;
}

//
// Member functions
//

logger &logger::instance()
{
  static logger _logger;
  return _logger;
}

//
// Builder
//

logger::builder &logger::builder::level(oak::level log_level)
{
  this->log_level = log_level;
  return *this;
}

logger::builder &logger::builder::file(std::string log_file)
{
  this->log_file = log_file;
  return *this;
}

subsystem &logger::builder::build()
{
  logger::log_level = this->log_level;
  logger::log_file = this->log_file;
  return logger::instance();
}
