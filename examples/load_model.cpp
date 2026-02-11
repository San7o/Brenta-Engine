// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

/**
 * Load a model and render it on the screen.
 */

#include <brenta/engine.hpp>
#include <brenta/logger.hpp>
#include <brenta/window.hpp>
#include <brenta/renderer/model.hpp>
#include <brenta/renderer/opengl/gl.hpp>
#include <brenta/renderer/opengl/shader.hpp>

#include <viotecs/viotecs.hpp>
REGISTER_SYSTEMS()

#include <iostream>

using namespace brenta;

int main()
{    
  const int screen_width = 800;
  const int screen_height = 600;

  //
  // Setup
  //

  Engine::Builder()
    .with(Logger::Builder()
          .level(oak::level::debug))
    .with(Window::Builder()
          .title("load model test")
          .width(screen_width)
          .height(screen_height))
    .with(Gl::Builder()
          .blending()
          .cull_face()
          .multisample()
          .depth_test())
    .build();
  auto engine = Engine::managed();

  //
  // Load assets
  //
  
  // Load the model
  Model our_model = Model::Builder()
    .path("examples/assets/models/backpack/backpack.obj")
    .build();

  // Load the shader
  Shader::create("default_shader",
                 Shader::Type::Vertex,   "examples/default_shader.vs",
                 Shader::Type::Fragment, "examples/default_shader.fs");

  //
  // Render loop
  //
  
  while (!Window::should_close())
  { 
    // Input
    if (Window::is_key_pressed(GLFW_KEY_ESCAPE))
      Window::close();

    // Clear
    Gl::set_color(0.2f, 0.2f, 0.207f, 1.0f);
    Gl::clear();

    // Draw
    Shader::use("default_shader");

    // Make transformations (you can also use brenta::translation)
    glm::mat4 view = glm::mat4(1.0f); // Camera position
    glm::mat4 projection =
      glm::perspective(glm::radians(45.0f),
                       (float) Window::get_width() / (float) Window::get_height(),
                       0.1f, 100.0f);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));

    Shader::set_mat4("default_shader", "view", view);
    Shader::set_mat4("default_shader", "projection", projection);
    Shader::set_mat4("default_shader", "model", model);

    // Draw the model
    our_model.draw("default_shader");

    Window::poll_events();
    Window::swap_buffers();
  }
  return 0;
}
