// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/gl.hpp>
#include <brenta/gui.hpp>
#include <brenta/window.hpp>
#include <brenta/texture.hpp>
#include <brenta/logger.hpp>

#ifdef BRENTA_USE_IMGUI

using namespace brenta;
using namespace brenta::types;

//
// Static variables
//

const std::string gui::subsystem_name = "gui";

//
// Subsystem interface
//

std::expected<void, subsystem::error> gui::initialize()
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  
  // Enable Keyboard and Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window::get_window(), true);
  ImGui_ImplOpenGL3_Init();
  ImGui::SetNextWindowPos(ImVec2(0, 0));

  INFO("GUI initialized");
  
  return {};
}

std::expected<void, subsystem::error> gui::terminate()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  INFO("GUI terminated");
  return {};
}

std::string gui::name()
{
  return gui::subsystem_name;
}

//
// Member functions
//

gui &gui::instance()
{
  static gui _gui;
  return _gui;
}

void gui::new_frame(framebuffer *fb, std::string name)
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

  //
  // Game window
  //

  ImGui::SetNextWindowSize(ImVec2(500, 500));
  ImGui::Begin(name.c_str());

  float window_width = ImGui::GetContentRegionAvail().x;
  float window_height = ImGui::GetContentRegionAvail().y;

  fb->rescale(window_width, window_height);
  gl::set_viewport(0, 0, window_width, window_height);

  ImGui::Image((void *) (intptr_t) fb->texture_id,
               ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

  ImGui::End();
}

void gui::render()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

//
// Builder
//

subsystem &gui::builder::build()
{
  return gui::instance();
}

#endif
