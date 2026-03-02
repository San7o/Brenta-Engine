// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/camera.hpp>
#include <brenta/ecs/ecs.hpp>

class CameraResource : public viotecs::Resource
{
public:
  
  tenno::shared_ptr<brenta::Camera> cam;

  CameraResource() = default;
  CameraResource(tenno::shared_ptr<brenta::Camera> c)
    : cam(c) {}
};
