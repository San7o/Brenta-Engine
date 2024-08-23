/*
 * MIT License
 *
 * Copyright (c) 2024 Giovanni Santini

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#pragma once

namespace Brenta
{

/**
 * @brief Time subsystem
 *
 * This subsystem is used to manage the time of the engine. It is used to
 * get the current time, the time since the last frame and the frames per
 * second of the program.
 */
class Time
{
  public:
    Time () = delete;

    /**
     * @brief Get the current time
     *
     * This function returns the current time in seconds since the start of the
     * program.
     *
     * @return The current time in seconds
     */
    static float GetCurrentTime ();
    /**
     * @brief Get the time since the last frame
     *
     * This function returns the time since the last update in seconds.
     *
     * @return The time since the last frame in seconds
     */
    static float GetDeltaTime ();
    /**
     * @brief Get the frames per second
     *
     * This function returns the frames per second of the program.
     *
     * @return The frames per second
     */
    static float GetFPS ();
    /**
     * @brief Update the time
     *
     * This function updates the time of the program. It's intended
     * use is to be called at each tick of the game loop.
     *
     * @param newTime The new time of the program
     */
    static void Update (float newTime);

  private:
    static float currentTime;
    static float deltaTime;
    static float lastFrame;
};

} // namespace Brenta
