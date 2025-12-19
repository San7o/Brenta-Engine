// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/camera.hpp>
#include <viotecs/viotecs.hpp>

using namespace viotecs;

struct CameraResource : resource
{
  brenta::camera *cam;
  CameraResource(brenta::camera *c) : cam(c)
  {
  }
};
