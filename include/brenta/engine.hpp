// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/subsystem.hpp>

#include <functional>

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
class engine : public subsystem
{
public:

  class manager;
  class builder;
  
  // Subsystem interface
  static const std::string subsystem_name;
  std::expected<void, subsystem::error> initialize() override;
  std::expected<void, subsystem::error> terminate() override;
  std::string name() override;
  bool is_initialized() override;

  // Constructors / destructors
  engine() = default;
  ~engine() = default;

  // Member functions
  
  // Get a static object instance
  static engine &instance();
  static engine::manager managed();

  // Initialize a subsystem and add it to the managed subsystems (will
  // be terminated with the others).
  static std::expected<void, std::string>
  with(subsystem::builder &&builder);

private:

  static std::vector<std::reference_wrapper<subsystem>> subsystems;
  static bool initialized;

};

/**
 * @brief Automatically initialize and terminate engine with RAII
 */
class engine::manager
{
public:
  /**
   * @brief Initializes all subsystems, throws and exeption in case of failure
   */
  manager();
  /**
   * @brief Terminates all subsystems
   */
  ~manager();
};

class engine::builder : public subsystem::builder
{
public:

  builder() = default;
  ~builder() = default;
  
  builder &with(subsystem::builder &builder);
  builder &with(subsystem::builder &&builder);
  
  brenta::subsystem &build() override;
  
private:
  
  std::vector<std::reference_wrapper<brenta::subsystem>> subsystems;
   
};

} // namespace brenta
