// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/model.hpp>
#include <brenta/renderer/light.hpp>

namespace brenta
{

class Node : public std::enable_shared_from_this<Node>
{
public:

  Node() = default;

  void add_model(std::shared_ptr<Model> model);
  void add_point_light(std::shared_ptr<PointLight> point_light);
  void set_dir_light(std::shared_ptr<DirLight> dir_light);
  
  void set_local(Transform local);
  Transform &get_local();

  // Update logic (animations, movement, AI...)
  void update(float delta_time);

  void update_world_matrix();
  // Render frame
  void draw();

  std::shared_ptr<Node> new_node();
  
private:

  std::vector<std::shared_ptr<Model>>          models;
  std::vector<std::shared_ptr<PointLight>>     point_lights;
  std::optional<std::shared_ptr<DirLight>>     dir_light;

  std::optional<std::weak_ptr<Node>>    parent;
  std::vector<std::shared_ptr<Node>>    children;

  Transform local;
  glm::mat4 world_matrix = glm::mat4(1.0f);
  
};

} // namespace brenta
