// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <expected>
#include <string>

namespace brenta
{

/**
 * @brief Subsystem interface
 *
 * A subsistem is a static singleton that can be initialized and
 * terminated. The initialization settings should be passed via a
 * builder class.
 */
class subsystem
{
public:

  using error = std::string;
  
  class builder;
  
  virtual ~subsystem() = default;
  
  virtual std::expected<void, subsystem::error> initialize() = 0;
  virtual std::expected<void, subsystem::error> terminate() = 0;
  /**
   * @brief Returns the name of the sybsystem
   */
  virtual std::string name() = 0;
};

/**
 * @brief Builder interface
 *
 * Interface to build a subsystem. It does not initialize it.
 */
class subsystem::builder
{
public:
  virtual ~builder() = default;
  
  virtual subsystem &build() = 0;
};

} // namepsace brenta
