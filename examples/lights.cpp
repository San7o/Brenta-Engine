// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#include <brenta/logger.hpp>
#include <brenta/window.hpp>
#include <brenta/scene.hpp>
#include <brenta/gui.hpp>
#include <brenta/node_components/dir_light_node_component.hpp>
#include <brenta/node_components/point_light_node_component.hpp>
#include <brenta/node_components/model_node_component.hpp>
#include <brenta/renderer/model.hpp>
#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/renderer.hpp>
#include <brenta/renderer/phong.hpp>
#include <brenta/renderer/pipeline.hpp>
#include <brenta/renderer/opengl/framebuffer.hpp>
#include <brenta/renderer/passes/opaque_pass.hpp>
#include <brenta/renderer/passes/transparent_pass.hpp>
#include <brenta/renderer/passes/ui_pass.hpp>

#include <tenno/memory.hpp>
#include <tenno/utility.hpp>

#include <iostream>

#include "../src/renderer/shaders/c/phong_vs.c"
#include "../src/renderer/shaders/c/phong_fs.c"

using namespace brenta;

#define ROTATION_SPEED 2.0f
void rotate_model_counterclockwise(tenno::shared_ptr<Model> model);
void rotate_model_clockwise(tenno::shared_ptr<Model> model);

void setup_gui(FrameBuffer &fb,
               tenno::shared_ptr<PhongDirLight> dir_light,
               tenno::shared_ptr<Node>            point_light_node,
               tenno::shared_ptr<PhongPointLight> point_light);

int main()
{
  Engine::Builder()
    .with(Logger::Builder()
          .event(Logger::Event::Lifetime)
          .level(Logger::Level::Debug))
    .with(Window::Builder()
          .title("load model test")
          .width(1280)
          .height(720))
    .with(Gl::Builder()
          .blending()
          .backface_culling()
          .multisample()
          .depth_test())
    .with(Gui::Builder())
    .build();
  auto engine = Engine::managed();

  auto maybe_shader = Shader::create({
      { Shader::Type::Vertex,   phong_vs },
      { Shader::Type::Fragment, phong_fs } });
  if (!maybe_shader)
  {
    ERROR("Error creating shader");
    return 1;
  }
  auto shader = tenno::move(maybe_shader.value());
  auto material = Material(tenno::move(shader));
  material.set_float("material.shininess", 50.0f); // 32.0f

  auto phong_dir =
    PhongDirLight()
    .set_strength(0.5f);
  auto phong_dir_ptr =
    tenno::make_shared<PhongDirLight>(tenno::move(phong_dir));

  auto phong_point =
    PhongPointLight()
    .set_strength(1.8f);
  auto phong_point_ptr =
    tenno::make_shared<PhongPointLight>(tenno::move(phong_point));

  auto camera_builder =
    Camera::Builder()
    .projection_type(Camera::ProjectionType::Perspective)
    .position(Camera::Aircraft::Builder()
              .pos({0.0f, 0.0f, 0.0f})
              .build())
    .fov(30.0f);
  auto scene = Scene(camera_builder);
  auto root_node = scene.get_root();

  auto model_builder =
    Model::Builder()
    .path("examples/assets/models/backpack/backpack.obj")
    .transform(Transform()
               .translate(glm::vec3(15.0f, 0.0f, 0.0f))
               .rotate_y(-90.0f)
               .scale(glm::vec3(1.0)))
    .texture_props(Texture::Properties()
                   .set_flipped(true))
    .material(tenno::move(material));
  auto model_component =
    tenno::make_shared<ModelNodeComponent>(model_builder);
  auto model_ptr = model_component->model;
  Scene::add_component(root_node, model_component);
  auto dir_light_component =
    tenno::make_shared<DirLightNodeComponent>(phong_dir_ptr);
  Scene::add_component(root_node, dir_light_component);

  auto point_light_node = Scene::create_child(root_node);
  auto point_light_component =
    tenno::make_shared<PointLightNodeComponent>(phong_point_ptr);
  Scene::add_component(point_light_node, point_light_component);

  auto game_fb = tenno::make_shared<FrameBuffer>(Window::get_width(),
                                                 Window::get_height());
    
  auto pipeline = tenno::make_shared<RenderPipeline>();
  pipeline->add_pass<OpaquePass>(game_fb, true, true);
  pipeline->add_pass<TransparentPass>(game_fb);
  pipeline->add_pass<UiPass>(game_fb);
  
  // Gui
  Gui::load_font();
  FrameBuffer fb(Window::get_width(), Window::get_height());  

  while (!Window::should_close())
  { 
    if (Window::is_key_pressed(Key::Escape))
      Window::close();
    if (Window::is_key_pressed(Key::Right))
      rotate_model_counterclockwise(model_ptr);
    if (Window::is_key_pressed(Key::Left))
      rotate_model_clockwise(model_ptr);

    setup_gui(*game_fb, phong_dir_ptr, point_light_node, phong_point_ptr);
    Gl::set_color(Color::grey());
    Gl::clear();
    
    // Render to framebuffer
    scene.update(Window::get_time().delta);
    scene.draw(pipeline, game_fb->width, game_fb->height);

    // Render to screen
    Window::framebuffer->bind();
    Gui::render();
    Window::framebuffer->unbind();
    
    Window::poll_events();
    Window::swap_buffers();
  }
  return 0;
}

void rotate_model_counterclockwise(tenno::shared_ptr<Model> model)
{
  model->transform.rotate_y(ROTATION_SPEED);
}

void rotate_model_clockwise(tenno::shared_ptr<Model> model)
{
  model->transform.rotate_y(-ROTATION_SPEED);
}

void setup_gui(FrameBuffer &fb,
               tenno::shared_ptr<PhongDirLight> dir_light,
               tenno::shared_ptr<Node>            point_light_node,
               tenno::shared_ptr<PhongPointLight> point_light)
{
  Gui::new_frame(&fb, "Lighting");
  Gui::push_font();
  ImGui::Begin("Lighting Settings");

  if (ImGui::CollapsingHeader("Directional Light",
                              ImGuiTreeNodeFlags_DefaultOpen))
  {
    ImGui::SeparatorText("Direction");

    glm::vec3 &dir = dir_light->direction;
    if (ImGui::SliderFloat3("Direction", &dir.x, -1.0f, 1.0f))
    {
      if (glm::length(dir) > 0.0001f)
        dir = glm::normalize(dir);
    }

    ImGui::SeparatorText("Intensity");
    ImGui::SliderFloat("Strength##Dir",
                       &dir_light->strength,
                       0.0f, 10.0f);
    ImGui::SeparatorText("Phong");
    ImGui::ColorEdit3("Ambient##Dir",
                      &dir_light->ambient.x);
    ImGui::ColorEdit3("Diffuse##Dir",
                      &dir_light->diffuse.x);
    ImGui::ColorEdit3("Specular##Dir",
                        &dir_light->specular.x);
    }

  if (ImGui::CollapsingHeader("Point Light",
                              ImGuiTreeNodeFlags_DefaultOpen))
  {
    ImGui::SeparatorText("Position");
    float pos[3] = {
      point_light_node->transform.position.x,
      point_light_node->transform.position.y,
      point_light_node->transform.position.z,
    };
    ImGui::SliderFloat3("Position",
                        pos,
                        -20.0f, 20.0f);
    point_light_node->transform.set_pos({pos[0], pos[1], pos[2]});
    
    ImGui::SeparatorText("Intensity");
    ImGui::SliderFloat("Strength##Point",
                       &point_light->strength,
                       0.0f, 50.0f,
                       "%.3f",
                       ImGuiSliderFlags_Logarithmic);

    ImGui::SeparatorText("Attenuation");
    ImGui::SliderFloat("Constant",
                         &point_light->constant,
                         0.0f, 2.0f);
    ImGui::SliderFloat("Linear",
                       &point_light->linear,
                       0.0f, 1.0f,
                       "%.5f");
    ImGui::SliderFloat("Quadratic",
                       &point_light->quadratic,
                       0.0f, 0.1f,
                       "%.6f",
                       ImGuiSliderFlags_Logarithmic);

    ImGui::SeparatorText("Phong");
    ImGui::ColorEdit3("Ambient##Point",
                      &point_light->ambient.x);
    ImGui::ColorEdit3("Diffuse##Point",
                      &point_light->diffuse.x);
    ImGui::ColorEdit3("Specular##Point",
                      &point_light->specular.x);
  }

  ImGui::End();
  Gui::pop_font();
}
