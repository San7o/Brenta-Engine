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
  static const std::string subsystem_name;

  // Subsystem interface
  std::expected<void, subsystem::error> initialize() override;
  std::expected<void, subsystem::error> terminate() override;
  std::string name() override;
  
  // Constructors / destructors
  ecs() = default;
  ~ecs() = default;

  // Member functions
  
  static ecs &instance();
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
