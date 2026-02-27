// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/subsystem.hpp>

#include <tenno/vector.hpp>
#include <tenno/functional.hpp>

#include <vector>

namespace brenta
{

/**
 * @brief Engine class
 *
 * This class is used to initialize and terminate multiple
 * subsystems. You can use the builder class to initialize the engine,
 * as with any other subsystem.
 *
 * Note: The subsystems will be initialized in the order they were
 * added, and terminated in reverse order. Make sure that they are
 * ordered correctly if a subsystem depends on another one.
 */
class Engine : public Subsystem
{
public:

  class Manager;
  class Builder;
  
  // Subsystem interface
  static const std::string subsystem_name;
  std::expected<void, Subsystem::Error> initialize() override;
  std::expected<void, Subsystem::Error> terminate() override;
  std::string name() override;
  bool is_initialized() override;

  Engine() = default;
  ~Engine() = default;
  
  // Member functions
  
  // Get a static object instance
  static Engine &instance();
  static Engine::Manager managed();

  // Initialize a subsystem and add it to the managed subsystems (will
  // be terminated with the others).
  static std::expected<void, std::string>
  with(Subsystem::Builder &&builder);

private:

  static tenno::vector<tenno::reference_wrapper<Subsystem>> subsystems;
  static bool initialized;

};

// Automatically initialize and terminate engine with RAII
class Engine::Manager
{
public:
  // Initializes all subsystems, throws and exeption in case of failure
  Manager();
  // Terminates all subsystems
  ~Manager();
};

class Engine::Builder : public Subsystem::Builder
{
public:

  Builder() = default;
  ~Builder() = default;
  
  Builder &with(Subsystem::Builder &builder);
  Builder &with(Subsystem::Builder &&builder);
  
  brenta::Subsystem &build() override;
  
private:
  
  tenno::vector<tenno::reference_wrapper<brenta::Subsystem>> subsystems;
   
};

} // namespace brenta
