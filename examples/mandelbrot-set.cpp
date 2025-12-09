// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

/**
 * Just a simple window
 */

#include <brenta/engine.hpp>
#include <iostream>
#include <viotecs/viotecs.hpp>

using namespace brenta;
using namespace brenta::types;

REGISTER_SYSTEMS()

const int SCR_WIDTH = 1280;
const int SCR_HEIGHT = 720;

int main()
{
  engine eng = engine::builder()
                 .use_screen(true)
                 .set_screen_width(SCR_WIDTH)
                 .set_screen_height(SCR_HEIGHT)
                 .set_screen_is_mouse_captured(false)
                 .use_logger(true)
                 .build();

  // A square
  float vertices[] = {// First Triangle
                      -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,

                      // Second Triangle
                      -1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f};
  vao v;
  v.init();
  buffer vbo = buffer(GL_ARRAY_BUFFER);
  vbo.copy_data(sizeof(vertices), vertices, GL_STATIC_DRAW);
  v.set_vertex_data(vbo, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

  shader::create("fractal", GL_VERTEX_SHADER, "examples/mandelbrot.vs",
                 GL_FRAGMENT_SHADER, "examples/mandelbrot.fs");

  framebuffer fb(SCR_WIDTH, SCR_HEIGHT);

  float zoom = 1.0f;
  glm::vec3 offset = glm::vec3(-0.11f, -0.11f, 0.0f);
  glm::vec3 constant = glm::vec3(0.350f, 0.467f, 0.0f);
  bool animate = false;
  bool julia = false;
  float animation_speed = 0.5;
  int max_iterations = 100;

  while (!screen::is_window_closed())
  {
    screen::poll_events();
    if (screen::is_key_pressed(GLFW_KEY_ESCAPE))
      screen::set_close();
    // Use arrows to move the fractal
    if (screen::is_key_pressed(GLFW_KEY_Z))
      zoom += 0.01f;
    if (screen::is_key_pressed(GLFW_KEY_X))
      zoom -= 0.01f;
    if (screen::is_key_pressed(GLFW_KEY_LEFT))
      offset.x -= 0.005f / pow(zoom, 4.0);
    if (screen::is_key_pressed(GLFW_KEY_RIGHT))
      offset.x += 0.005f / pow(zoom, 4.0);
    if (screen::is_key_pressed(GLFW_KEY_UP))
      offset.y += 0.005f / pow(zoom, 4.0);
    if (screen::is_key_pressed(GLFW_KEY_DOWN))
      offset.y -= 0.005f / pow(zoom, 4.0);

    // Vary constant over time
    if (animate)
    {
      constant.x = sin(screen::get_time() * animation_speed);
      constant.y = cos(screen::get_time() * animation_speed);
    }

#ifdef BRENTA_USE_IMGUI
    gui::new_frame(&fb);
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
    gl::clear();

    // Render here
    v.bind();
    shader::use("fractal");
    shader::set_vec3("fractal", "resolution",
                     glm::vec3(float(SCR_WIDTH), float(SCR_HEIGHT), 0.0f));
    shader::set_vec3("fractal", "offset", offset);
    shader::set_float("fractal", "zoom", zoom);
    shader::set_vec3("fractal", "constant", constant);
    shader::set_bool("fractal", "juliaSet", julia);
    shader::set_int("fractal", "maxIterations", max_iterations);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    gl::check_error();

    v.unbind();
    fb.unbind();
#ifdef BRENTA_USE_IMGUI
    gui::render();
#endif

    screen::swap_buffers();
  }

  return 0;
}
