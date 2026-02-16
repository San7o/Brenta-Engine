// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <viotecs/viotecs.hpp>

using namespace viotecs;

struct WireframeResource : Resource
{
  bool enabled;
  
  WireframeResource(bool e) : enabled(e)
  {
  }
};
