// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/logger.hpp>

using namespace brenta;

oak::level logger::log_level;
std::string logger::log_file;

logger &logger::instance()
{
  static logger _logger;
  return _logger;
}

std::expected<void, std::string> logger::initialize()
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
    INFO("Set log file: {}", log_file);
  }

  return {};
}

std::expected<void, std::string> logger::terminate()
{
  oak::stop_writer();
  return {};
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
