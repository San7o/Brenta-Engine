// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

namespace brenta
{

class Time
{
public:

  float elapsed;
  float delta;
  float last_frame;
  
  Time() = default;

  float get_fps();
  
  // updates its data
  void update(float new_time);
  
};

} // namespace brenta
