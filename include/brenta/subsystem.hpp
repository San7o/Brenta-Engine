// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <expected>
#include <string>

namespace brenta
{

//
// Subsystem interface
// -------------------
//
// A subsistem is a static singleton that can be initialized and
// terminated. The initialization settings should be passed via a
// builder class.
//
class Subsystem
{
public:

  using Error = std::string;
  
  class Builder;
  
  virtual ~Subsystem() = default;
  
  virtual std::expected<void, Subsystem::Error> initialize() = 0;
  virtual std::expected<void, Subsystem::Error> terminate()  = 0;
  
  // Returns the name of the sybsystem
  virtual std::string name()    = 0;
  // Returns true if the subsystem is initialized
  virtual bool is_initialized() = 0;
};

//
// Builder interface
//
// Interface to build a subsystem. It does not initialize it.
//
class Subsystem::Builder
{
public:
  virtual ~Builder() = default;
  
  virtual Subsystem &build() = 0;
};

} // namepsace brenta
