// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifdef BRENTA_USE_ECS

#pragma once

#include <brenta/subsystem.hpp>
#include <viotecs/viotecs.hpp>

namespace brenta
{

class ecs : public subsystem
{
public:

  class builder;

  // Subsystem interface
  static const std::string subsystem_name;
  std::expected<void, subsystem::error> initialize() override;
  std::expected<void, subsystem::error> terminate() override;
  std::string name() override;
  bool is_initialized() override;
  
  // Constructors / destructors
  ecs() = default;
  ~ecs() = default;

  // Member functions
  
  static ecs &instance();

private:

  static bool initialized;

};

class ecs::builder : public subsystem::builder
{
public:

  builder() = default;
  ~builder() = default;

  subsystem &build();
};
  
} // namespace brenta

#endif // BRENTA_USE_ECS
