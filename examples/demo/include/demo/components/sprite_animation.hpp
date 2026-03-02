// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/ecs/ecs.hpp>

class SpriteAnimationEcsComponent : public viotecs::Component
{
public:

  int atlas_size   = 0;
  int atlas_index  = 0;
  int duration     = 60; // in frames
  int time_passed  = 0;  // keep the number of frames since last change

  SpriteAnimationEcsComponent() = default;
  SpriteAnimationEcsComponent(int size, int index, int duration)
    : atlas_size(size), atlas_index(index), duration(duration) {}
  
};
