// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

namespace brenta
{

class Mouse
{
public:

  Mouse() = default;
  Mouse(float x, float y) : x(x), y(y) {}
  Mouse(float x, float y, float sensitivity)
    : x(x), y(y), sensitivity(sensitivity) {}
  
  // Getters
  
  bool  get_first() const;
  float get_x() const;
  float get_y() const;
  float get_sensitivity() const;

  // Setters
  
  void set_first(bool first);
  void set_x(float last_x);
  void set_y(float last_y);
  void set_sensitivity(float sensitivity);

private:
  
  // First is useful if you want to set the initial position of
  // the mouse as "invalid" instead of (0, 0)
  bool    first = true;
  float   x;
  float   y;
  float   sensitivity;
};

} // namespace brenta
