// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/camera.hpp>

#include <viotecs/viotecs.hpp>

using namespace viotecs;

struct CameraResource : Resource
{
  tenno::shared_ptr<brenta::Camera> cam;
  CameraResource(tenno::shared_ptr<brenta::Camera> c) : cam(c)
  {
  }
};
