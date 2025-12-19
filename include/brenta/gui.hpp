// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#ifdef BRENTA_USE_IMGUI

#include <brenta/frame_buffer.hpp>
#include <brenta/subsystem.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace brenta
{

/**
 * @brief Gui class
 *
 * This class contains the GUI functions, It's a wrapper around imgui,
 * providing functions to initialize, update and delete the gui.
 */
class gui : public subsystem
{
public:

  class builder;

  static const std::string subsystem_name;

  // Subsystem interface
  std::expected<void, subsystem::error> initialize() override;
  std::expected<void, subsystem::error> terminate() override;
  std::string name() override;
  bool is_initialized() override;

  // Costructors / destructors
  gui() = default;
  ~gui() = default;

  // Member functions
  
  static gui &instance();
  
  /**
   * @brief Start a new frame
   * To be called at each frame before rendering.
   */
  static void new_frame(types::framebuffer *fb, std::string name = "Game");
  /**
   * @brief Render the gui
   * To be called at each frame after rendering.
   */
  static void render();

private:
  
  static bool initialized;
  
};

class gui::builder : public subsystem::builder
{
public:

  builder() = default;
  ~builder() = default;
  
  brenta::subsystem &build() override;
  
};
  
} // namespace brenta

#endif // BRENTA_USE_IMGUI
