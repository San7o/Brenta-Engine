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
protected:
  
  static oak::level log_level;
  static std::string log_file;
  
public:

  std::string subsystem_name = "logger";
  
  logger() = default;
  ~logger() = default;

  static logger &instance();
  
  std::expected<void, std::string> initialize() override;
  std::expected<void, std::string> terminate() override;

  class builder;
};

class logger::builder : public subsystem::builder
{
private:

  oak::level log_level = oak::level::info;;
  std::string log_file = "/tmp/brenta_logs";
  
public:

  builder() = default;
  ~builder() = default;

  builder &level(oak::level log_level);
  builder &file(std::string out_file);
  
  subsystem &build();
};
  
} // namespace brenta
