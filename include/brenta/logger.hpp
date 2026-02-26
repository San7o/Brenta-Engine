// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/subsystem.hpp>
#include <oak/oak.hpp>

#include <fstream>
#include <expected>
#include <string>
#include <filesystem>

#define DEBUG(...)       OAK_DEBUG(__VA_ARGS__);
#define INFO(...)        OAK_INFO(__VA_ARGS__);
#define WARN(...)        OAK_WARN(__VA_ARGS__);
#define ERROR(...)       OAK_ERROR(__VA_ARGS__);
#define EVENT(type, ...) OAK_EVENT(type, __VA_ARGS__)

namespace brenta
{

class Logger : public Subsystem
{
public:

  enum Event
  {
    Lifetime,
    Callback,
  };

  using Level = oak::Level;
  using Flags  = oak::Flags;
  
  struct Config;
  class Builder;
  
  // Subsystem interface
  static const std::string subsystem_name;
  std::expected<void, Subsystem::Error> initialize() override;
  std::expected<void, Subsystem::Error> terminate() override;
  std::string name() override;
  bool is_initialized() override;

  // Member functions
  
  static Logger &instance();
  static std::string event_name(enum Event event);
  
private:
  
  static Config         init_config;
  static bool           initialized;

  // Private constructors / destructors for singleton  
  Logger() = default;
  ~Logger() = default;
  
};

struct Logger::Config
{
  oak::Level            log_level = oak::Level::Info;
  std::filesystem::path log_file  = "/tmp/brenta-logs.txt";
  std::vector<Event>    events    = {};
  std::vector<Flags>    flags     = {};
};

class Logger::Builder : public Subsystem::Builder
{
private:

  Logger::Config conf = {};
  
public:

  Builder()  = default;
  ~Builder() = default;

  Builder &level(Logger::Level log_level);
  Builder &file(std::filesystem::path out_file);
  Builder &event(Logger::Event event);
  Builder &flag(Logger::Flags flag);
  
  Subsystem &build();
};
  
} // namespace brenta
