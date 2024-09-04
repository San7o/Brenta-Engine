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

#include "engine_time.hpp"

#include "screen.hpp"

using namespace brenta;

float time::current_time = 0.0f;
float time::delta_time = 0.0f;
float time::last_frame = 0.0f;

float time::get_current_time()
{
    return screen::get_time();
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
