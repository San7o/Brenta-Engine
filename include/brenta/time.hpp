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
  Time() = default;
  
  // updates the internal data of this object. It is mainly called
  // at each tick
  void update(float new_time);

  // returns the elapsed time since the start of the program in seconds
  float get_elapsed();
  // returns the times since the last frame in seconds
  float get_delta();
  float get_fps();
  
private:
  
  float elapsed;
  float delta;
  float last_frame;
};

} // namespace brenta
