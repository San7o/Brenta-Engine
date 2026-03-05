// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

namespace brenta
{

//
// Mouse
// -----
//
// This is an handy class to keep track of the state of the mouse.
// You probably would have created this class anyway, so the engine
// provides a default one for convenience.
//
class Mouse
{
public:

  // `first` is useful if you want to set the initial position of the
  // mouse as "invalid" instead of (0, 0)
  bool    first = true;
  float   x;
  float   y;
  float   sensitivity;
  
  Mouse() = default;
  Mouse(float x, float y) : x(x), y(y) {}
  Mouse(float x, float y, float sensitivity)
    : x(x), y(y), sensitivity(sensitivity) {}

};

} // namespace brenta
