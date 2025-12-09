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

  std::string subsystem_name = "ecs";
  
  ecs() = default;
  ~ecs() = default;

  static ecs &instance();
  
  std::expected<void, std::string> initialize() override;
  std::expected<void, std::string> terminate() override;

  class builder;
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
