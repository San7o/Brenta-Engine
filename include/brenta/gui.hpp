// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#ifndef BRENTA_NO_IMGUI

#include <brenta/renderer/opengl/framebuffer.hpp>
#include <brenta/subsystem.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <filesystem>

namespace brenta
{

//
// Gui class
// ---------
//
// This class contains some utilities for working with GUI. It's a
// wrapper around imgui, providing common functions to initialize,
// update and delete the gui. This is not meant to be a replacement
// for ImGui's API, but a shortcut to some some common operations
// that may take many keystrokes to type out.
//
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

  // Member functions
  
  static Gui &instance();
  
  static void new_frame(FrameBuffer *fb, std::string name = "Game");
  static void render();
  static void load_font(const std::filesystem::path &path = "examples/assets/fonts/Inconsolata-Regular.ttf",
                        float size = 25.0f);
  static void push_font(ImFont* f = font);
  static void pop_font();

private:
  
  static bool    initialized;
  static ImFont *font;
  
  // Private constructors / destructors for singleton
  Gui()  = default;
  ~Gui() = default;
  
};

class Gui::Builder : public Subsystem::Builder
{
public:

  Builder()  = default;
  ~Builder() = default;
  
  brenta::Subsystem &build() override;
  
};
  
} // namespace brenta

#endif // BRENTA_NO_IMGUI
