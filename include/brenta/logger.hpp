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

#define DEBUG(...) OAK_DEBUG(__VA_ARGS__);
#define INFO(...) OAK_INFO(__VA_ARGS__);
#define WARN(...) OAK_WARN(__VA_ARGS__);
#define ERROR(...) OAK_ERROR(__VA_ARGS__);

namespace brenta
{

class logger : public subsystem
{
public:

  struct config;
  class builder;
  
  static const std::string subsystem_name;
  static const config default_config;
  static config init_config;

  // Subsystem interface
  std::expected<void, subsystem::error> initialize() override;
  std::expected<void, subsystem::error> terminate() override;
  std::string name() override;
  bool is_initialized() override;
  
  // Constructors / destructors
  logger() = default;
  ~logger() = default;

  // Member functions
  
  static logger &instance();
  
private:
  
  static bool initialized;
  
};

struct logger::config
{
  oak::level log_level;
  std::string log_file;
};

class logger::builder : public subsystem::builder
{
private:

  logger::config conf = logger::default_config;
  
public:

  builder() = default;
  ~builder() = default;

  builder &level(oak::level log_level);
  builder &file(std::string out_file);
  
  subsystem &build();
};
  
} // namespace brenta
