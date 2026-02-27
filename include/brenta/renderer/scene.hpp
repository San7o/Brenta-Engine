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
  
}  // namespace brenta
