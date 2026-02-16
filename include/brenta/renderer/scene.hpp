// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/node.hpp>

#pragma once

namespace brenta
{

class Scene
{
public:

  Scene(std::shared_ptr<Camera> camera)
    : active_camera(camera)
  {
    this->root = std::make_shared<Node>();
  }

  std::shared_ptr<Node>   get_root() const;
  std::shared_ptr<Camera> get_active_camera() const;
  
  void update(float delta_time);
  void draw();
  
private:

  std::shared_ptr<Node>   root;
  std::shared_ptr<Camera> active_camera;
  
};
  
}  // namespace brenta
