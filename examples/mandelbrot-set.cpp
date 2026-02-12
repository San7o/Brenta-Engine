// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/brenta.hpp>

#include <viotecs/viotecs.hpp>
REGISTER_SYSTEMS()

#include <iostream>

#include "assets/shaders/c/mandelbrot_vs.c"
#include "assets/shaders/c/mandelbrot_fs.c"

using namespace brenta;

int main()
{
  //
  // Setup
  //
  
  Engine::Builder()
    .with(Logger::Builder()
          .level(oak::level::debug)
          .file("/tmp/brenta-logs"))
    .with(Window::Builder()
          .title("mandelbrot set")
          .width(800)
          .height(600)
          .vsync()
          .debug())
    .with(Gl::Builder()
          .blending()
          .cull_face()
          .multisample()
          .depth_test())
    .with(Input::Builder())
    .with(Gui::Builder())
    .build();
  auto engine = Engine::managed();

  // A square
  float vertices[] = {// First Triangle
                      -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,

                      // Second Triangle
                      -1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f};
  Vao v;
  v.init();
  v.bind();
  Buffer vbo;
  vbo.init(GL_ARRAY_BUFFER);
  vbo.bind();
  vbo.copy_data(sizeof(vertices), vertices, GL_STATIC_DRAW);
  v.set_vertex_data(vbo, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

  auto shader = Shader::create("fractal", {
       { Shader::Type::Vertex, mandelbrot_vs },
       { Shader::Type::Fragment, mandelbrot_fs } });
  if (!shader)
  {
    ERROR("Error creating shader");
    return 1;
  }

  FrameBuffer fb(Window::get_width(), Window::get_height());

  float     zoom            = 1.0f;
  glm::vec3 offset          = glm::vec3(-0.11f, -0.11f, 0.0f);
  glm::vec3 constant        = glm::vec3(0.350f, 0.467f, 0.0f);
  bool      animate         = false;
  bool      julia           = false;
  float     animation_speed = 0.5;
  int       max_iterations  = 100;

  while (!Window::should_close())
  {
    Window::poll_events();
    if (Window::is_key_pressed(GLFW_KEY_ESCAPE))
      Window::close();
    // Use arrows to move the fractal
    if (Window::is_key_pressed(GLFW_KEY_Z))
      zoom += 0.01f;
    if (Window::is_key_pressed(GLFW_KEY_X))
      zoom -= 0.01f;
    if (Window::is_key_pressed(GLFW_KEY_LEFT))
      offset.x -= 0.005f / pow(zoom, 4.0);
    if (Window::is_key_pressed(GLFW_KEY_RIGHT))
      offset.x += 0.005f / pow(zoom, 4.0);
    if (Window::is_key_pressed(GLFW_KEY_UP))
      offset.y += 0.005f / pow(zoom, 4.0);
    if (Window::is_key_pressed(GLFW_KEY_DOWN))
      offset.y -= 0.005f / pow(zoom, 4.0);

    // Vary constant over time
    if (animate)
    {
      constant.x = sin(Window::get_time().get_elapsed() * animation_speed);
      constant.y = cos(Window::get_time().get_elapsed() * animation_speed);
    }

#ifdef BRENTA_USE_IMGUI
    Gui::new_frame(&fb, "Mandlebrot");
    ImGui::Begin("Fractal");
    ImGui::SliderFloat("Zoom", &zoom, 0.0f, 10.0f);
    ImGui::SliderInt("Max iterations", &max_iterations, 1, 1000);
    ImGui::SliderFloat("Offset X", &offset.x, -2.0f, 2.0f);
    ImGui::SliderFloat("Offset Y", &offset.y, -2.0f, 2.0f);
    ImGui::Text("Only for julia set:");
    ImGui::Checkbox("Use Julia set", &julia);
    ImGui::SliderFloat("Constant X", &constant.x, -2.0f, 2.0f);
    ImGui::SliderFloat("Constant Y", &constant.y, -2.0f, 2.0f);
    ImGui::Checkbox("Animate", &animate);
    ImGui::SliderFloat("Animation speed", &animation_speed, 0.0f, 2.0f);
    ImGui::End();
#endif
    fb.bind();
    Gl::clear();

    // Render here
    v.bind();

    shader->use();
    shader->set_vec3("resolution",
                     glm::vec3(float(Window::get_width()),
                               float(Window::get_height()), 0.0f));
    shader->set_vec3("offset",       offset);
    shader->set_float("zoom",        zoom);
    shader->set_vec3("constant",     constant);
    shader->set_bool("juliaSet",     julia);
    shader->set_int("maxIterations", max_iterations);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    Gl::check_error();

    v.unbind();
    fb.unbind();
#ifdef BRENTA_USE_IMGUI
    Gui::render();
#endif

    Window::swap_buffers();
  }
  return 0;
}
