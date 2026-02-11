// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/subsystem.hpp>
#include <oak/oak.hpp>

#include <fstream>
#include <iostream>
#include <expected>
#include <string>
#include <filesystem>

#define DEBUG(...)  OAK_DEBUG(__VA_ARGS__);
#define INFO(...)   OAK_INFO(__VA_ARGS__);
#define WARN(...)   OAK_WARN(__VA_ARGS__);
#define ERROR(...)  OAK_ERROR(__VA_ARGS__);

namespace brenta
{

class Logger : public Subsystem
{
public:

  struct Config;
  class Builder;
  
  // Subsystem interface
  static const std::string subsystem_name;
  std::expected<void, Subsystem::Error> initialize() override;
  std::expected<void, Subsystem::Error> terminate() override;
  std::string name() override;
  bool is_initialized() override;
  
  // Constructors / destructors
  Logger() = default;
  ~Logger() = default;

  // Member functions
  
  static Logger &instance();
  
private:
  
  static const Config   default_config;
  static Config         init_config;
  static bool           initialized;
  
};

struct Logger::Config
{
  oak::level            log_level;
  std::filesystem::path log_file;
};

class Logger::Builder : public Subsystem::Builder
{
private:

  Logger::Config conf = Logger::default_config;
  
public:

  Builder()  = default;
  ~Builder() = default;

  Builder &level(oak::level log_level);
  Builder &file(std::filesystem::path out_file);
  
  Subsystem &build();
};
  
} // namespace brenta
