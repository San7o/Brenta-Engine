// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

//
// Create a model from a list of vertexes
//

#include <brenta/engine.hpp>
#include <brenta/input.hpp>
#include <brenta/window.hpp>
#include <brenta/logger.hpp>
#include <brenta/renderer/mesh.hpp>
#include <brenta/renderer/opengl/gl.hpp>
#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/model.hpp>
#include <brenta/renderer/renderer.hpp>
#include <brenta/renderer/pipeline.hpp>

#include "../src/renderer/shaders/c/phong_vs.c"
#include "../src/renderer/shaders/c/phong_fs.c"

#include <tenno/memory.hpp>

using namespace brenta;

int main(void)
{
  Engine::Builder()
    .with(Logger::Builder()
          .level(Logger::Level::Debug))
    .with(Window::Builder()
          .title("Vetex Mesh Example")
          .debug())
    .with(Input::Builder())
    .with(Gl::Builder()
          .backface_culling()
          .depth_test())
    .build();
  auto engine = Engine::managed();

  // At the start, the camera looks at the X axis
  auto camera =
    Camera::Builder()
    .position(Camera::Aircraft::Builder()
              .pos({0.0f, 0.0f, 0.0f})
              .build())
    .build();

  auto maybe_shader = Shader::create({
      { Shader::Type::Vertex,   phong_vs },
      { Shader::Type::Fragment, phong_fs },
    });
  if (!maybe_shader)
  {
    ERROR("Error creating shader");
    return 1;
  }
  auto shader = tenno::move(maybe_shader.value());
  auto material =
    tenno::make_shared<Material>(tenno::move(shader));
  auto texture_builder =
    Texture::Builder()
    .type(Texture::Type::Diffuse)
    .path("examples/assets/textures/container2.png");
  auto texture =
    tenno::make_shared<Texture>(texture_builder.build());

  //
  // Shapes
  //

  // Triangle
  
  auto triangle_builder =
    Model::Builder()
    .transform(Transform()
               .translate(glm::vec3(5.0f, 1.0f, 0.0f))
               .rotate(glm::angleAxis(glm::radians(-90.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f)))
               .scale(glm::vec3(0.5f)))
    .mesh(Mesh::Builder()
          .shape(Mesh::Shape::Triangle)
          .texture(texture))
    .material(material);
  auto triangle =
    tenno::make_shared<Model>(triangle_builder);

  // Square
  
  auto square_builder =
    Model::Builder()
    .transform(Transform()
               .translate(glm::vec3(5.0f, 1.0f, 1.5f))
               .rotate(glm::angleAxis(glm::radians(-90.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f)))
               .scale(glm::vec3(0.5f)))
    .mesh(Mesh::Builder()
          .shape(Mesh::Shape::Square)
          .texture(texture))
    .material(material);
  auto square =
    tenno::make_shared<Model>(square_builder);

  // Circle

  auto circle_builder =
    Model::Builder()
    .transform(Transform()
               .translate(glm::vec3(5.0f, 1.0f, -1.5f))
               .rotate(glm::angleAxis(glm::radians(-90.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f)))
               .scale(glm::vec3(0.5f)))
    .mesh(Mesh::Builder()
          .shape(Mesh::Shape::Circle)
          .texture(texture))
    .material(material);
  auto circle =
    tenno::make_shared<Model>(circle_builder);
  
  // Pyramid
  
  auto pyramid_builder =
    Model::Builder()
    .transform(Transform()
               .translate(glm::vec3(5.0f, -1.0f, 0.0f))
               .rotate(glm::angleAxis(glm::radians(-45.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f)))
               .scale(glm::vec3(0.5f)))
    .mesh(Mesh::Builder()
          .shape(Mesh::Shape::Pyramid)
          .texture(texture))
    .material(material);
  auto pyramid =
    tenno::make_shared<Model>(pyramid_builder);

  // Cube
  
  auto cube_builder =
    Model::Builder()
    .transform(Transform()
               .translate(glm::vec3(5.0f, -1.0f, 1.5f))
               .rotate(glm::angleAxis(glm::radians(-90.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f)))
               .scale(glm::vec3(0.5f)))
    .mesh(Mesh::Builder()
          .shape(Mesh::Shape::Cube)
          .texture(texture))
    .material(material);
  auto cube =
    tenno::make_shared<Model>(cube_builder);

  // Sphere
  
  auto sphere_builder =
    Model::Builder()
    .transform(Transform()
               .translate(glm::vec3(5.0f, -1.0f, -1.5f))
               .rotate(glm::angleAxis(glm::radians(-90.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f)))
               .scale(glm::vec3(0.5f)))
    .mesh(Mesh::Builder()
          .shape(Mesh::Shape::Sphere)
          .texture(texture))
    .material(material);
  auto sphere =
    tenno::make_shared<Model>(sphere_builder);
  
  auto pipeline = RenderPipeline::create_default();

  bool wireframe = false;
  Input::add_keyboard_callback(Key::Space, [&wireframe]() {
    wireframe = !wireframe;
    Gl::set_poligon_mode(wireframe);
  });
  
  while(!Window::should_close())
  {
    if (Window::is_key_pressed(Key::Escape))
      Window::close();
    
    Gl::set_color(Color::grey());
    Gl::clear();

    // Draw
    Renderer::begin_frame(camera, Window::get_width(), Window::get_height());
    Renderer::submit({glm::mat4(1.0f), triangle});
    Renderer::submit({glm::mat4(1.0f), square});
    Renderer::submit({glm::mat4(1.0f), circle});
    Renderer::submit({glm::mat4(1.0f), pyramid});
    Renderer::submit({glm::mat4(1.0f), cube});
    Renderer::submit({glm::mat4(1.0f), sphere});
    Renderer::end_frame(pipeline);
    
    Window::poll_events();
    Window::swap_buffers();
  }
  
  return 0;
}
