// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#ifdef BRENTA_USE_IMGUI

#include <brenta/renderer/opengl/framebuffer.hpp>
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
class Gui : public Subsystem
{
public:

  class Builder;

  // Subsystem interface
  static const std::string subsystem_name;
  std::expected<void, Subsystem::Error> initialize() override;
  std::expected<void, Subsystem::Error> terminate() override;
  std::string name() override;
  bool is_initialized() override;

  // Costructors / destructors
  Gui() = default;
  ~Gui() = default;

  // Member functions
  
  static Gui &instance();
  
  static void new_frame(FrameBuffer *fb, std::string name = "Game");
  static void render();

private:
  
  static bool initialized;
  
};

class Gui::Builder : public Subsystem::Builder
{
public:

  Builder() = default;
  ~Builder() = default;
  
  brenta::Subsystem &build() override;
  
};
  
} // namespace brenta

#endif // BRENTA_USE_IMGUI
