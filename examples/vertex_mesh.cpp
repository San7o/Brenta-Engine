// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/mesh.hpp>
#include <brenta/engine.hpp>
#include <brenta/window.hpp>
#include <brenta/renderer/opengl/gl.hpp>
#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/model.hpp>
#include <brenta/renderer/renderer.hpp>
#include <brenta/logger.hpp>

#include <viotecs/viotecs.hpp>
REGISTER_SYSTEMS();

#include "assets/shaders/c/default_shader_vs.c"
#include "assets/shaders/c/default_shader_fs.c"

using namespace brenta;

int main(void)
{
  Engine::Builder()
    .with(Logger::Builder()
          .level(oak::level::debug))
    .with(Window::Builder()
          .title("Vetex Mesh Example")
          .debug())
    .with(Gl::Builder()
          .cull_face()
          .depth_test())
    .build();
  auto engine = Engine::managed();

  // At the start, the camera looks at the X axis
  auto camera = Camera::Builder()
    .position(Camera::Aircraft::Builder()
              .pos({0.0f, 0.0f, 0.0f})
              .build())
    .build();

  auto model = Model::Builder()
    .transform(Transform()
               // Move the model forward in the X axis, and rotate it
               // so it can be seen by the camera
               .translate(glm::vec3(5.0f, 0.0f, 0.0f))
               .rotate(glm::angleAxis(glm::radians(-90.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f)))
               .scale(glm::vec3(1.0f, 1.0f, 1.0f)))
    .mesh(Mesh::Builder()
          .vertices({
              // A simple triangle
              { glm::vec3(-0.5f, -0.5f, 0.0), glm::vec3(1.0), glm::vec2(0.0, 0.0) },
              { glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(1.0), glm::vec2(1.0, 0.0) },
              { glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(1.0),  glm::vec2(0.5, 1.0) },
            })
          .indices({0, 1 , 2})
          .texture(Texture::Builder()
                   .type(Texture::Type::Diffuse)
                   .path("examples/assets/textures/container2.png")
                   .build())
          .build())
    .build();

  auto shader = Shader::create("my_shader", {
      { Shader::Type::Vertex,   default_shader_vs },
      { Shader::Type::Fragment, default_shader_fs },
    });
  if (!shader)
  {
    ERROR("Error creating shader");
    return 1;
  }
  
  while(!Window::should_close())
  {
    if (Window::is_key_pressed(Key::Escape))
      Window::close();

    Gl::set_color(Color::grey());
    Gl::clear();

    // Update

    auto pos = camera.get_pos();
    auto acam = std::get<Camera::Aircraft>(pos);
    acam.yaw++;
    if (acam.yaw >= 45.0f)
      acam.yaw = -45.0f;
    camera.set_pos(acam);
    
    // Draw
    Renderer::begin_frame(camera);
    Renderer::submit({&model, Material(*shader)});
    Renderer::end_frame();
    
    Window::poll_events();
    Window::swap_buffers();
  }
  
  return 0;
}
