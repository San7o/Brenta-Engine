// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#ifndef BRENTA_NO_ECS

#include <brenta/subsystem.hpp>
#include <viotecs/viotecs.hpp>

namespace brenta
{

//
// Ecs
// ---
//
// The engine uses `viotecs` to handle all the ecs logic. This
// subsystems is just a way to initialize the ECS.
//
class Ecs : public Subsystem
{
public:

  class Builder;

  // Subsystem interface
  static const std::string subsystem_name;
  std::string name()    override;
  bool is_initialized() override;
  std::expected<void, Subsystem::Error> initialize() override;
  std::expected<void, Subsystem::Error> terminate()  override;
  
  // Member functions
  
  static Ecs &instance();

private:

  static bool initialized;
  
  // Private constructors / destructors for singleton
  Ecs()  = default;
  ~Ecs() = default;

};

class Ecs::Builder : public Subsystem::Builder
{
public:

  Builder()  = default;
  ~Builder() = default;

  Subsystem &build();
};
  
} // namespace brenta

#endif // BRENTA_NO_ECS
