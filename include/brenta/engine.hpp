// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/audio.hpp>
#include <brenta/buffer.hpp>
#include <brenta/camera.hpp>
#include <brenta/mouse.hpp>
#include <brenta/frame_buffer.hpp>
#include <brenta/gl.hpp>
#include <brenta/gui.hpp>
#include <brenta/input.hpp>
#include <brenta/logger.hpp>
#include <brenta/mesh.hpp>
#include <brenta/model.hpp>
#include <brenta/particles.hpp>
#include <brenta/window.hpp>
#include <brenta/shader.hpp>
#include <brenta/text.hpp>
#include <brenta/texture.hpp>
#include <brenta/time.hpp>
#include <brenta/translation.hpp>
#include <brenta/vao.hpp>
#include <brenta/subsystem.hpp>
#include <brenta/ecs.hpp>
#include <brenta/renderer.hpp>

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
 * Note: The subsystems will be initialized in the order as they are
 * added and terminated in reverse, so make sure that they are ordered
 * correctly if a subsystem depends on another one.
 */
class engine : public subsystem
{
public:

  class manager;
  class builder;
  
  static const std::string subsystem_name;
  
  // Subsystem interface
  std::expected<void, subsystem::error> initialize() override;
  std::expected<void, subsystem::error> terminate() override;
  std::string name() override;
  bool is_initialized() override;

  // Constructors / destructors
  engine() = default;
  ~engine() = default;

  // Member functions
  /**
   * @brief Get a static object instance
   */
  static engine &instance();
  static engine::manager managed();
  
  /**
   * @brief Initialize a subsystem and add it to the managed
   * subsystems (will be terminated with the others).
   */
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
  
/**
 * @brief Engine builder
 *
 * This class is used to build the engine.
 */
class engine::builder : public subsystem::builder
{
private:
  
  std::vector<std::reference_wrapper<brenta::subsystem>> subsystems;
  
public:

  builder() = default;
  ~builder() = default;
  
  builder &with(subsystem::builder &builder);
  builder &with(subsystem::builder &&builder);
  brenta::subsystem &build() override;
  
};

} // namespace brenta
