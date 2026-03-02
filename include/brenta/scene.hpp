// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/camera.hpp>
#include <brenta/node.hpp>

#include <tenno/memory.hpp>

#pragma once

namespace brenta
{

class Scene
{
public:

  class Builder;

  Scene(Camera&& camera);
  Scene(tenno::shared_ptr<Camera> camera);
  Scene(Camera::Builder& camera);

  tenno::shared_ptr<Node>   get_root() const;
  tenno::shared_ptr<Camera> get_camera() const;

  static void add_component(tenno::shared_ptr<Node> owner,
                            tenno::shared_ptr<NodeComponent> component);
  static tenno::shared_ptr<Node> create_child(tenno::shared_ptr<Node> parent);
  
  static void set_script(tenno::weak_ptr<Node> node, const std::filesystem::path &path);
  
  static void set_script(tenno::weak_ptr<Node> node, const std::string &source);
    
  void update(float delta_time);
  void draw();
  
private:

  tenno::shared_ptr<Node>   root;
  tenno::shared_ptr<Camera> active_camera;
  
};

class Scene::Builder
{
public:
  
  Builder &camera(tenno::shared_ptr<Camera> camera);
  Builder &camera(Camera&& camera);
  Builder &camera(Camera::Builder& camera);

  Scene build();

private:

  tenno::shared_ptr<Camera> _camera;
  
};
  
}  // namespace brenta
