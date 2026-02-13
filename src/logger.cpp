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
Logger::Config Logger::init_config = {};
bool Logger::initialized = false;

//
// Subsystem interface
//

std::expected<void, Subsystem::Error> Logger::initialize()
{
  if (this->is_initialized()) return {};
  
  oak::init_writer();
  
  oak::set_level(Logger::init_config.log_level);

  auto file_name = Logger::init_config.log_file;
  if (file_name != "")
  {
    auto file = oak::set_file(file_name);
    if (!file.has_value())
    {
      ERROR("{}: Failed to open log file: {}",
            Logger::subsystem_name, file_name.c_str());
      return std::unexpected(this->subsystem_name);
    }
    INFO("{}: set log file to {}", Logger::subsystem_name, file_name.c_str());
  }

  Logger::initialized = true;
  INFO("{}: initialized", Logger::subsystem_name);
  return {};
}

std::expected<void, Subsystem::Error> Logger::terminate()
{
  if (!this->is_initialized()) return {};
  
  oak::stop_writer();

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

Logger::Builder &Logger::Builder::level(oak::level log_level)
{
  this->conf.log_level = log_level;
  return *this;
}

Logger::Builder &Logger::Builder::file(std::filesystem::path log_file)
{
  this->conf.log_file = log_file;
  return *this;
}

Subsystem &Logger::Builder::build()
{
  Logger::init_config = this->conf;
  return Logger::instance();
}
