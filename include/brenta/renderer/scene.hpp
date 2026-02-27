// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/node.hpp>

#include <tenno/memory.hpp>

#pragma once

namespace brenta
{

class Scene
{
public:

  class Builder;
  
  Scene(tenno::shared_ptr<Camera> camera)
    : active_camera(camera)
  {
    this->root = tenno::make_shared<Node>();
  }

  tenno::shared_ptr<Node>   get_root() const;
  tenno::shared_ptr<Camera> get_active_camera() const;

  static tenno::shared_ptr<Node> create_child(tenno::shared_ptr<Node> parent);
  
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

  Scene build();

private:

  tenno::shared_ptr<Camera> _camera;
  
};
  
}  // namespace brenta
