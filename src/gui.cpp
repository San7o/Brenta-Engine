// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/opengl/gl.hpp>
#include <brenta/gui.hpp>
#include <brenta/window.hpp>
#include <brenta/logger.hpp>

#include <cmath>

#ifndef BRENTA_NO_IMGUI

namespace brenta
{

struct TimePlotBuffer
{
  int max_size;
  int offset;
  ImVector<ImVec2> data;
  
  TimePlotBuffer(int max_size = 2000)
  {
    this->max_size = max_size;
    this->offset   = 0;
    this->data.reserve(max_size);
  }
  
  void add_point(float x, float y)
  {
    if (this->data.size() < this->max_size)
      this->data.push_back(ImVec2(x,y));
    else {
      this->data[this->offset] = ImVec2(x,y);
      this->offset =  (this->offset + 1) % this->max_size;
    }
  }
  
  void erase()
  {
    if (this->data.size() > 0)
    {
      this->data.shrink(0);
      this->offset  = 0;
    }
  }
};

} // namespace brenta

using namespace brenta;

//
// Static variables
//

const std::string Gui::subsystem_name = "gui";
bool              Gui::initialized = false;
ImFont           *Gui::font = nullptr;

static void setup_style()
{
  ImGuiStyle& style  = ImGui::GetStyle();
  ImVec4*     colors = style.Colors;

  // Color palette
  const ImVec4 base       = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);       // #000000
  const ImVec4 mantle     = ImVec4(0.169f, 0.169f, 0.169f, 1.0f); // #2b2b2b
  const ImVec4 surface0   = ImVec4(0.169f, 0.169f, 0.169f, 1.0f); // #2b2b2b
  const ImVec4 surface1   = ImVec4(0.100f, 0.100f, 0.100f, 1.0f); // 
  const ImVec4 surface2   = ImVec4(0.050f, 0.050f, 0.050f, 1.0f); // 
  const ImVec4 overlay0   = ImVec4(0.396f, 0.403f, 0.486f, 1.0f); // #65677c
  const ImVec4 overlay2   = ImVec4(0.576f, 0.584f, 0.654f, 1.0f); // #9399b2
  const ImVec4 text       = ImVec4(0.803f, 0.815f, 0.878f, 1.0f); // #cdd6f4
  const ImVec4 subtext0   = ImVec4(0.639f, 0.658f, 0.764f, 1.0f); // #a3a8c3
  const ImVec4 mauve      = ImVec4(0.796f, 0.698f, 0.972f, 1.0f); // #cba6f7
  const ImVec4 peach      = ImVec4(0.980f, 0.709f, 0.572f, 1.0f); // #fab387
  const ImVec4 yellow     = ImVec4(0.980f, 0.913f, 0.596f, 1.0f); // #f9e2af
  const ImVec4 green      = ImVec4(0.650f, 0.890f, 0.631f, 1.0f); // #a6e3a1
  const ImVec4 teal       = ImVec4(0.580f, 0.886f, 0.819f, 1.0f); // #94e2d5
  const ImVec4 sapphire   = ImVec4(0.458f, 0.784f, 0.878f, 1.0f); // #74c7ec
  const ImVec4 blue       = ImVec4(0.533f, 0.698f, 0.976f, 1.0f); // #89b4fa
  const ImVec4 lavender   = ImVec4(0.709f, 0.764f, 0.980f, 1.0f); // #b4befe

  // Main window and backgrounds
  colors[ImGuiCol_WindowBg]             = base;
  colors[ImGuiCol_ChildBg]              = base;
  colors[ImGuiCol_PopupBg]              = surface0;
  colors[ImGuiCol_Border]               = surface1;
  colors[ImGuiCol_BorderShadow]         = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
  colors[ImGuiCol_FrameBg]              = surface0;
  colors[ImGuiCol_FrameBgHovered]       = surface1;
  colors[ImGuiCol_FrameBgActive]        = surface2;
  colors[ImGuiCol_TitleBg]              = mantle;
  colors[ImGuiCol_TitleBgActive]        = surface0;
  colors[ImGuiCol_TitleBgCollapsed]     = mantle;
  colors[ImGuiCol_MenuBarBg]            = mantle;
  colors[ImGuiCol_ScrollbarBg]          = surface0;
  colors[ImGuiCol_ScrollbarGrab]        = surface2;
  colors[ImGuiCol_ScrollbarGrabHovered] = overlay0;
  colors[ImGuiCol_ScrollbarGrabActive]  = overlay2;
  colors[ImGuiCol_CheckMark]            = green;
  colors[ImGuiCol_SliderGrab]           = sapphire;
  colors[ImGuiCol_SliderGrabActive]     = blue;
  colors[ImGuiCol_Button]               = surface0;
  colors[ImGuiCol_ButtonHovered]        = surface1;
  colors[ImGuiCol_ButtonActive]         = surface2;
  colors[ImGuiCol_Header]               = surface0;
  colors[ImGuiCol_HeaderHovered]        = surface1;
  colors[ImGuiCol_HeaderActive]         = surface2;
  colors[ImGuiCol_Separator]            = surface1;
  colors[ImGuiCol_SeparatorHovered]     = mauve;
  colors[ImGuiCol_SeparatorActive]      = mauve;
  colors[ImGuiCol_ResizeGrip]           = surface2;
  colors[ImGuiCol_ResizeGripHovered]    = mauve;
  colors[ImGuiCol_ResizeGripActive]     = mauve;
  colors[ImGuiCol_Tab]                  = surface0;
  colors[ImGuiCol_TabHovered]           = surface2;
  colors[ImGuiCol_TabActive]            = surface1;
  colors[ImGuiCol_TabUnfocused]         = surface0;
  colors[ImGuiCol_TabUnfocusedActive]   = surface1;
  colors[ImGuiCol_DockingPreview]       = sapphire;
  colors[ImGuiCol_DockingEmptyBg]       = base;
  colors[ImGuiCol_PlotLines]            = blue;
  colors[ImGuiCol_PlotLinesHovered]     = peach;
  colors[ImGuiCol_PlotHistogram]        = teal;
  colors[ImGuiCol_PlotHistogramHovered] = green;
  colors[ImGuiCol_TableHeaderBg]        = surface0;
  colors[ImGuiCol_TableBorderStrong]    = surface1;
  colors[ImGuiCol_TableBorderLight]     = surface0;
  colors[ImGuiCol_TableRowBg]           = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
  colors[ImGuiCol_TableRowBgAlt]        = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
  colors[ImGuiCol_TextSelectedBg]       = surface2;
  colors[ImGuiCol_DragDropTarget]       = yellow;
  colors[ImGuiCol_NavHighlight]         = lavender;
  colors[ImGuiCol_NavWindowingHighlight]= ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
  colors[ImGuiCol_NavWindowingDimBg]    = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
  colors[ImGuiCol_ModalWindowDimBg]     = ImVec4(0.0f, 0.0f, 0.0f, 0.35f);
  colors[ImGuiCol_Text]                 = text;
  colors[ImGuiCol_TextDisabled]         = subtext0;

  // Rounded corners
  style.WindowRounding    = 6.0f;
  style.ChildRounding     = 6.0f;
  style.FrameRounding     = 4.0f;
  style.PopupRounding     = 4.0f;
  style.ScrollbarRounding = 9.0f;
  style.GrabRounding      = 4.0f;
  style.TabRounding       = 4.0f;

  // Padding and spacing
  style.WindowPadding     = ImVec2(8.0f, 8.0f);
  style.FramePadding      = ImVec2(5.0f, 3.0f);
  style.ItemSpacing       = ImVec2(8.0f, 4.0f);
  style.ItemInnerSpacing  = ImVec2(4.0f, 4.0f);
  style.IndentSpacing     = 21.0f;
  style.ScrollbarSize     = 14.0f;
  style.GrabMinSize       = 10.0f;

  // Borders
  style.WindowBorderSize  = 1.0f;
  style.ChildBorderSize   = 1.0f;
  style.PopupBorderSize   = 1.0f;
  style.FrameBorderSize   = 0.0f;
  style.TabBorderSize     = 0.0f;
}

//
// Subsystem interface
//

std::expected<void, Subsystem::Error> Gui::initialize()
{
  if (this->is_initialized()) return {};
  
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  
  // Enable Keyboard and Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)Window::get_window(), true);
  ImGui_ImplOpenGL3_Init();
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  setup_style();

  Gui::initialized = true;
  INFO("{} initialized", Gui::subsystem_name);
  return {};
}

std::expected<void, Subsystem::Error> Gui::terminate()
{
  if (!this->is_initialized()) return {};
  
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();

  ImPlot::DestroyContext();
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

void Gui::load_font(const std::filesystem::path &path, float size)
{
  ImGuiIO& io  = ImGui::GetIO();
  Gui::font = io.Fonts->AddFontFromFileTTF(path.string().c_str(), size);
}

void Gui::push_font(ImFont *f)
{
  ImGui::PushFont(f);
}

void Gui::pop_font()
{
  ImGui::PopFont();
}

void Gui::new_frame(FrameBuffer &fb, std::string name)
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

  float window_width  = ImGui::GetContentRegionAvail().x;
  float window_height = ImGui::GetContentRegionAvail().y;

  fb.rescale(window_width, window_height);

  ImGui::Image((void *) (intptr_t) fb.texture_id,
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

static float gui_debug_last_time = 0;

void Gui::debug_performance()
{
  ImGui::Begin("Performance");

  static TimePlotBuffer fps_data;

  auto time = Window::get_time();
  const float interval  = 0.1f; // seconds

  float fps  = time.get_fps();
  
  if (time.elapsed > gui_debug_last_time + interval)
  {
    gui_debug_last_time = time.elapsed;
    fps_data.add_point(time.elapsed, fps);
  }

  const ImPlotAxisFlags flags = 0;
  static float history        = 10.0f;
  ImGui::SliderFloat("History", &history, 1, 60, "%.1f s");
    
  if (ImPlot::BeginPlot("##FPS", ImVec2(-1,ImGui::GetTextLineHeight()*10)))
  {
    ImPlot::SetupAxes(nullptr, nullptr, flags, flags);

    // X axis limit
    ImPlot::SetupAxisLimits(ImAxis_X1, gui_debug_last_time - history,
                            gui_debug_last_time, ImGuiCond_Always);
    // Y axis limit
    ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 100);
    
    ImPlotSpec spec;
    spec.Offset = fps_data.offset;
    spec.Stride = 2 * sizeof(float);
    spec.FillAlpha = 0.5f;
    ImPlot::PlotShaded("FPS", &fps_data.data[0].x, &fps_data.data[0].y,
                       fps_data.data.size(), -INFINITY, spec);
    ImPlot::EndPlot();
  }
  
  ImGui::End();
}

//
// Builder
//

Subsystem &Gui::Builder::build()
{
  return Gui::instance();
}

#endif // BRENTA_NO_IMGUI
