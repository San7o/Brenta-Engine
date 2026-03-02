// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/ecs/ecs.hpp>

class WireframeResource : public viotecs::Resource
{
public:
  
  bool enabled = false;

  WireframeResource() = default;
  WireframeResource(bool e) : enabled(e) {}
  
};
