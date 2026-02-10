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

#include <iostream>
#include <viotecs/viotecs.hpp>

REGISTER_SYSTEMS()

using namespace brenta;
  
const int screen_width = 800;
const int screen_height = 600;

int main()
{
  //
  // Setup
  //

  engine::builder()
    .with(logger::builder()
          .level(oak::level::debug))
    .with(window::builder()
          .title("load model test")
          .width(screen_width)
          .height(screen_height))
    .with(gl::builder()
          .blending()
          .cull_face()
          .multisample()
          .depth_test())
    .build();
  auto engine = engine::managed();

  //
  // Load assets
  //
  
  // Load the model
  model our_model = model::builder()
    .path("examples/assets/models/backpack/backpack.obj")
    .build();

  // Load the shader
  shader::create("default_shader",
                 shader::type::vertex, "examples/default_shader.vs",
                 shader::type::fragment, "examples/default_shader.fs");

  //
  // Render loop
  //
  
  while (!window::should_close())
  { 
    // Input
    if (window::is_key_pressed(GLFW_KEY_ESCAPE))
      window::close();

    // Clear
    gl::set_color(0.2f, 0.2f, 0.207f, 1.0f);
    gl::clear();

    // Draw
    shader::use("default_shader");

    // Make transformations (you can also use brenta::translation)
    glm::mat4 view = glm::mat4(1.0f); // Camera position
    glm::mat4 projection =
      glm::perspective(glm::radians(45.0f),
                       (float) window::get_width() / (float) window::get_height(),
                       0.1f, 100.0f);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));

    shader::set_mat4("default_shader", "view", view);
    shader::set_mat4("default_shader", "projection", projection);
    shader::set_mat4("default_shader", "model", model);

    // Draw the model
    our_model.draw("default_shader");

    window::poll_events();
    window::swap_buffers();
  }
  return 0;
}
