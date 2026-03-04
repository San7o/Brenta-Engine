// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/time.hpp>

using namespace brenta;

float Time::get_fps()
{
  return 1.0f / this->delta;
}

void Time::update(float new_time)
{
  this->elapsed    = new_time;
  this->delta      = this->elapsed - this->last_frame;
  this->last_frame = this->elapsed;
}
