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
#include <brenta/renderer/scene.hpp>
#include <brenta/renderer/phong.hpp>
#include <brenta/renderer/opengl/framebuffer.hpp>
#include <brenta/gui.hpp>

#include <tenno/memory.hpp>
#include <tenno/utility.hpp>

#include <iostream>

#include "../src/renderer/shaders/c/phong_vs.c"
#include "../src/renderer/shaders/c/phong_fs.c"

using namespace brenta;

#define ROTATION_SPEED 2.0f
void rotate_model_counterclockwise(tenno::shared_ptr<Model> model);
void rotate_model_clockwise(tenno::shared_ptr<Model> model);

void setup_gui(FrameBuffer &fb, ImFont *font,
               tenno::shared_ptr<PhongDirLight> dir_light,
               tenno::shared_ptr<PhongPointLight> point_light);

int main()
{
  const int screen_width = 1280;
  const int screen_height = 720;

  Engine::Builder()
    .with(Logger::Builder()
          .event(Logger::Event::Lifetime)
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
    .with(Gui::Builder())
    .build();
  auto engine = Engine::managed();
  
  auto camera =
    tenno::make_shared<Camera>(Camera::Builder()
                               .projection_type(Camera::ProjectionType::Perspective)
                               .position(Camera::Aircraft::Builder()
                                         .pos({0.0f, 0.0f, 0.0f})
                                         .build())
                               .fov(30.0f)
                               .build());

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
  material->set_float("material.shininess", 32.0f);

  auto model =
    tenno::make_shared<Model>(Model::Builder()
                              .path("examples/assets/models/backpack/backpack.obj")
                              .transform(Transform()
                                         .translate(glm::vec3(15.0f, 0.0f, 0.0f))
                                         .rotate_y(-90.0f)
                                         .scale(glm::vec3(1.0)))
                              .texture_props(Texture::Properties()
                                             .flipped(true))
                              .material(material)
                              .build());

  auto phong_dir =
    tenno::make_shared<PhongDirLight>(PhongDirLight()
                                      .set_strength(0.5f));
  auto phong_point =
    tenno::make_shared<PhongPointLight>(PhongPointLight()
                                        .set_strength(1.8f));
  
  auto scene = Scene(camera);
  auto root_node = scene.get_root();
  root_node->add_model(model);
  root_node->add_point_light(phong_point);
  root_node->set_dir_light(phong_dir);

  // Gui
  ImGuiIO& io = ImGui::GetIO();
  ImFont* arial = io.Fonts->AddFontFromFileTTF("examples/assets/fonts/arial.ttf",
                                               30.0f);
  FrameBuffer fb(Window::get_width(), Window::get_height());  

  while (!Window::should_close())
  { 
    if (Window::is_key_pressed(Key::Escape))
      Window::close();
    if (Window::is_key_pressed(Key::Right))
      rotate_model_counterclockwise(model);
    if (Window::is_key_pressed(Key::Left))
      rotate_model_clockwise(model);

    setup_gui(fb, arial, phong_dir, phong_point);
    
    Gl::set_color(Color::grey());
    Gl::clear();
    
    // Render to framebuffer
    fb.bind();
    scene.update(Window::get_time().get_delta());
    scene.draw();
    fb.unbind();

    // Render to screen
    Gui::render();
    
    Window::poll_events();
    Window::swap_buffers();
  }
  return 0;
}

void rotate_model_counterclockwise(tenno::shared_ptr<Model> model)
{
  auto& transform = model->get_transform();
  transform.rotate_y(ROTATION_SPEED);
}

void rotate_model_clockwise(tenno::shared_ptr<Model> model)
{
  auto& transform = model->get_transform();
  transform.rotate_y(-ROTATION_SPEED);
}

void setup_gui(FrameBuffer &fb, ImFont *font,
               tenno::shared_ptr<PhongDirLight> dir_light,
               tenno::shared_ptr<PhongPointLight> point_light)
{
  Gui::new_frame(&fb, "Lighting");
  ImGui::PushFont(font);
  ImGui::Begin("Lighting Settings");

  if (ImGui::CollapsingHeader("Directional Light",
                              ImGuiTreeNodeFlags_DefaultOpen))
  {
    ImGui::SeparatorText("Direction");

    glm::vec3 &dir = dir_light->get_direction();
    if (ImGui::SliderFloat3("Direction", &dir.x, -1.0f, 1.0f))
    {
      if (glm::length(dir) > 0.0001f)
        dir = glm::normalize(dir);
    }

    ImGui::SeparatorText("Intensity");
    ImGui::SliderFloat("Strength##Dir",
                       &dir_light->get_strength(),
                       0.0f, 10.0f);
    ImGui::SeparatorText("Phong");
    ImGui::ColorEdit3("Ambient##Dir",
                      &dir_light->get_ambient().x);
    ImGui::ColorEdit3("Diffuse##Dir",
                      &dir_light->get_diffuse().x);
    ImGui::ColorEdit3("Specular##Dir",
                        &dir_light->get_specular().x);
    }

  if (ImGui::CollapsingHeader("Point Light",
                              ImGuiTreeNodeFlags_DefaultOpen))
  {
    ImGui::SeparatorText("Position");
    ImGui::SliderFloat3("Position",
                        &point_light->get_position().x,
                        -20.0f, 20.0f);
    ImGui::SeparatorText("Intensity");
    ImGui::SliderFloat("Strength##Point",
                       &point_light->get_strength(),
                       0.0f, 50.0f,
                       "%.3f",
                       ImGuiSliderFlags_Logarithmic);

    ImGui::SeparatorText("Attenuation");
    ImGui::SliderFloat("Constant",
                         &point_light->get_constant(),
                         0.0f, 2.0f);
    ImGui::SliderFloat("Linear",
                       &point_light->get_linear(),
                       0.0f, 1.0f,
                       "%.5f");
    ImGui::SliderFloat("Quadratic",
                       &point_light->get_quadratic(),
                       0.0f, 0.1f,
                       "%.6f",
                       ImGuiSliderFlags_Logarithmic);

    ImGui::SeparatorText("Phong");
    ImGui::ColorEdit3("Ambient##Point",
                      &point_light->get_ambient().x);
    ImGui::ColorEdit3("Diffuse##Point",
                      &point_light->get_diffuse().x);
    ImGui::ColorEdit3("Specular##Point",
                      &point_light->get_specular().x);
  }

  ImGui::End();
  ImGui::PopFont();
}
