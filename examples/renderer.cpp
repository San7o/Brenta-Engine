// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#include <brenta/logger.hpp>
#include <brenta/window.hpp>
#include <brenta/renderer/model.hpp>
#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/renderer.hpp>
#include <brenta/renderer/opengl/gl.hpp>

#include "../src/renderer/shaders/c/phong_vs.c"
#include "../src/renderer/shaders/c/phong_fs.c"

#include <tenno/memory.hpp>
#include <tenno/utility.hpp>

using namespace brenta;

int main()
{
  const int screen_width = 800;
  const int screen_height = 600;

  Engine::Builder()
    .with(Logger::Builder()
          .level(Logger::Level::Debug))
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
  
  auto cam = Camera::Builder()
    .projection_type(Camera::ProjectionType::Perspective)
    .position(Camera::Aircraft::Builder()
              .pos({0.0f, 0.0f, 0.0f})
              .build())
    .fov(45.0f)
    .build();

  auto shader = Shader::create({
      { Shader::Type::Vertex,   phong_vs },
      { Shader::Type::Fragment, phong_fs } });
  if (!shader)
  {
    ERROR("Error creating shader");
    return 1;
  }
  auto shader_ptr = tenno::make_shared<Shader>(tenno::move(shader.value()));
  
  auto material = tenno::make_shared<Material>(shader_ptr);

  auto model = Model::Builder()
    .path("examples/assets/models/backpack/backpack.obj")
    .transform(Transform()
               .translate(glm::vec3(5.0f, 0.0f, 0.0f))
               .rotate(glm::angleAxis(glm::radians(-90.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f)))
               .scale(glm::vec3(1.0)))
    .texture_props(Texture::Properties()
                        .flipped(true))
    .material(material)
    .build();

  auto cam_ptr   = tenno::make_shared<Camera>(tenno::move(cam));
  auto model_ptr = tenno::make_shared<Model>(tenno::move(model));
  
  while (!Window::should_close())
  { 
    if (Window::is_key_pressed(Key::Escape))
      Window::close();

    Gl::set_color(Color::grey());
    Gl::clear();
    
    Renderer::begin_frame(cam_ptr);
    Renderer::submit({glm::mat4(1.0f), model_ptr});
    Renderer::end_frame();
    
    Window::poll_events();
    Window::swap_buffers();
  }
  return 0;
}
