// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#ifndef BRENTA_NO_ECS

#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/opengl/framebuffer.hpp>
#include <brenta/ecs/ecs.hpp>

#include <tenno/memory.hpp>

namespace brenta
{

class CameraEcsComponent : public viotecs::Component
{
public:
  
  tenno::shared_ptr<Camera>    camera   = nullptr;
  tenno::weak_ptr<FrameBuffer> fb;

  CameraEcsComponent() = default;
  CameraEcsComponent(tenno::shared_ptr<Camera> cam, tenno::weak_ptr<FrameBuffer> fb)
    : camera(cam), fb(fb) {}
  CameraEcsComponent(const Camera &cam, tenno::weak_ptr<FrameBuffer> fb)
    : fb(fb)
  {
    this->camera = tenno::make_shared<Camera>(cam);
  }
  CameraEcsComponent(Camera::Builder &cam, tenno::weak_ptr<FrameBuffer> fb)
    : fb(fb)
  {
    this->camera = tenno::make_shared<Camera>(cam.build());
  }
};
  
} // namespace brenta

#endif // BRENTA_NO_ECS
