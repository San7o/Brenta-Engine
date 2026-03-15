// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/particles.hpp>
#include <brenta/renderer/opengl/framebuffer.hpp>
#include <brenta/engine.hpp>
#include <brenta/input.hpp>
#include <brenta/time.hpp>
#include <brenta/logger.hpp>
#include <brenta/gui.hpp>
#include <brenta/window.hpp>
#include <brenta/mouse.hpp>

using namespace brenta;

void setup_gui(tenno::shared_ptr<FrameBuffer> fb);

int main()
{
  Engine::Builder()
    .with(Logger::Builder()
          .level(Logger::Level::Debug)
          .file("/tmp/brenta-logs"))
    .with(Window::Builder()
          .title("particles")
          .width(1280)
          .height(720)
          .msaa()
          .vsync()
          .debug())
    .with(Gl::Builder()
          .blending()
          .backface_culling()
          .multisample()
          .depth_test())
    .with(Gui::Builder())
    .with(Input::Builder())
    .build(); 
  auto engine = Engine::managed();

  auto camera_builder =
    Camera::Builder()
    .position(Camera::Spherical::Builder()
              .center({0.0f, 2.0f, 0.0f})
              .theta(1.25f)
              .phi(1.25f)
              .radius(10.0f)
              .build())
    .projection_type(Camera::ProjectionType::Perspective)
    .fov(45.0f);
  auto camera =
    tenno::make_shared<Camera>(camera_builder);

  Gui::load_font();
  auto fb = tenno::make_shared<FrameBuffer>(Window::get_width(),
                                            Window::get_height());  
  
  //
  // Render loop
  //

  while (!Window::should_close())
  {
    if (Window::is_key_pressed(Key::Escape))
      Window::close();

    setup_gui(fb);

    Gl::set_color(Color::grey());
    Gl::clear();

    fb->bind();
    Gl::clear();
    Gl::set_viewport(0, 0, fb->width, fb->height);
    // Draw is supposed to be here
    fb->unbind();

    Window::framebuffer->bind();
    Gui::render();
    Window::framebuffer->unbind();
    
    Window::poll_events();
    Window::swap_buffers();
  }
  return 0;
}

void setup_gui(tenno::shared_ptr<FrameBuffer> fb)
{
  Gui::new_frame(fb, "Game");
  Gui::debug_stats();
}
