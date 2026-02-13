// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/model.hpp>
#include <brenta/renderer/camera.hpp>

#pragma once

namespace brenta
{

class Scene
{
public:

  Scene() = default;

  Scene &add_model(std::shared_ptr<Model> model);
  Scene &set_active_camera(std::shared_ptr<Camera> camera);
  std::shared_ptr<Camera> get_active_camera() const;

  // Update logic (animations, movement, AI...)
  void update(float delta_time);
  // Render frame
  void draw();

private:

  std::vector<std::shared_ptr<Model>> models;
  std::shared_ptr<Camera>             active_camera;
  
};
  
}  // namespace brenta
