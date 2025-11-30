// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

namespace brenta
{

/**
 * @brief Time subsystem
 *
 * This subsystem is used to manage the time of the engine. It is used to
 * get the current time, the time since the last frame and the frames per
 * second of the program.
 */
class time
{
public:
  time() = delete;

  /**
   * @brief Get the current time
   *
   * This function returns the current time in seconds since the start of the
   * program.
   *
   * @return The current time in seconds
   */
  static float get_current_time();
  /**
   * @brief Get the time since the last frame
   *
   * This function returns the time since the last update in seconds.
   *
   * @return The time since the last frame in seconds
   */
  static float get_delta_time();
  /**
   * @brief Get the frames per second
   *
   * This function returns the frames per second of the program.
   *
   * @return The frames per second
   */
  static float get_fps();
  /**
   * @brief Update the time
   *
   * This function updates the time of the program. It's intended
   * use is to be called at each tick of the game loop.
   *
   * @param newTime The new time of the program
   */
  static void update(float new_time);

private:
  static float current_time;
  static float delta_time;
  static float last_frame;
};

} // namespace brenta
