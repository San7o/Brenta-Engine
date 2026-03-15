// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me

#pragma once

#include <brenta/renderer/camera.hpp>
#include <brenta/text.hpp>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <tenno/vector.hpp>
#include <tenno/memory.hpp>

namespace brenta
{

class Model;
class PointLight;
class DirLight;
class Skybox;
class RenderPipeline;
  
//
// Renderer
// --------
//
// The renderer is a logical abtraction that is responsible to draw a
// "rendering unit" aka render command. When the renderer is flushed,
// or when `end_frame()` is called, all render commands will be
// drawn.
//
// Its usage usually looks like this:
//
//    Renderer::begin_frame(camera);
//
//    Renderer::submit({world_matrix, model});
//    Renderer::submit({world_matrix2, model2});
//    Renderer::submit_point_light(light);
//    // ...
//
//    Renderer::end_frame();  // draws everything    
//
class Renderer
{
public:

  struct Command;
  struct RenderData;
  
  Renderer() = delete;
  ~Renderer() = delete;

  // You can begin a frame withouth a camera, but eventually you will
  // have to call set_camera if you want to see anything
  static void begin_frame();
  static void begin_frame(Camera &cam, int width, int height);
  
  static void set_camera(Camera &cam, int width, int height);
  
  // Transparent commands are rendered after non-transparent ones
  static void submit(const Renderer::Command& it, bool transparent = false);
  static void submit_point_light(tenno::shared_ptr<PointLight> point_light);
  static void submit_point_lights(const tenno::vector<tenno::shared_ptr<PointLight>>& point_light);
  static void submit_dir_light(tenno::shared_ptr<DirLight> dir_light);
  static void submit_text(const Text& text);
  static void submit_skybox(tenno::shared_ptr<Skybox> skybox);

  static void end_frame(tenno::shared_ptr<RenderPipeline> pipeline);

  // Draw and clear state
  static void flush(tenno::shared_ptr<RenderPipeline> pipeline);
  // Clear all state
  static void clear();

private:

  static RenderData data;

};
  
struct Renderer::Command
{
  glm::mat4                    world_matrix;
  tenno::shared_ptr<Model>     model;
  
  Command() = default;
  Command(glm::mat4 world_matrix,
          tenno::shared_ptr<Model> model)
    : world_matrix(world_matrix), model(model) {}
};

struct Renderer::RenderData
{
  glm::mat4   projection;
  glm::mat4   view;
  glm::vec3   cam_position;
  int         width;
  int         height;
  
  tenno::vector<Command>   opaque_queue;
  tenno::vector<Command>   transparent_queue;
  tenno::vector<Text>      ui_queue;
  
  tenno::vector<tenno::shared_ptr<PointLight>>    point_lights;
  std::optional<tenno::shared_ptr<DirLight>>      dir_light;
  std::optional<tenno::shared_ptr<Skybox>>        skybox;
};

} // namespace brenta
