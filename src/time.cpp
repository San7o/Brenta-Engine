// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/window.hpp>
#include <brenta/time.hpp>

using namespace brenta;

float time::current_time = 0.0f;
float time::delta_time = 0.0f;
float time::last_frame = 0.0f;

float time::get_current_time()
{
  return window::instance().get_time();
}

float time::get_delta_time()
{
  return delta_time;
}

float time::get_fps()
{
  return 1.0f / delta_time;
}

void time::update(float new_time)
{
  current_time = new_time;
  delta_time = current_time - last_frame;
  last_frame = current_time;
}
