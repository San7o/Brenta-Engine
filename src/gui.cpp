// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/opengl/gl.hpp>
#include <brenta/gui.hpp>
#include <brenta/window.hpp>
#include <brenta/logger.hpp>

#ifdef BRENTA_USE_IMGUI

using namespace brenta;

//
// Static variables
//

const std::string Gui::subsystem_name = "gui";
bool              Gui::initialized = false;

//
// Subsystem interface
//

std::expected<void, Subsystem::Error> Gui::initialize()
{
  if (this->is_initialized()) return {};
  
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  
  // Enable Keyboard and Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)Window::get_window(), true);
  ImGui_ImplOpenGL3_Init();
  ImGui::SetNextWindowPos(ImVec2(0, 0));

  Gui::initialized = true;
  INFO("{} initialized", Gui::subsystem_name);
  return {};
}

std::expected<void, Subsystem::Error> Gui::terminate()
{
  if (!this->is_initialized()) return {};
  
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  Gui::initialized = false;
  INFO("{}: terminated", Gui::subsystem_name);
  return {};
}

std::string Gui::name()
{
  return Gui::subsystem_name;
}

bool Gui::is_initialized()
{
  return Gui::initialized;
}

//
// Member functions
//

Gui &Gui::instance()
{
  static Gui _gui;
  return _gui;
}

void Gui::new_frame(FrameBuffer *fb, std::string name)
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

  //
  // Game window
  //

  ImGui::SetNextWindowSize(ImVec2(Window::get_width() * 0.8,
                                  Window::get_width() * 0.8),
                           ImGuiCond_FirstUseEver);
  ImGui::Begin(name.c_str());

  float window_width = ImGui::GetContentRegionAvail().x;
  float window_height = ImGui::GetContentRegionAvail().y;

  fb->rescale(window_width, window_height);
  Gl::set_viewport(0, 0, window_width, window_height);

  ImGui::Image((void *) (intptr_t) fb->texture_id,
               ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

  ImGui::End();
  return;
}

void Gui::render()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  return;
}

//
// Builder
//

Subsystem &Gui::Builder::build()
{
  return Gui::instance();
}

#endif
