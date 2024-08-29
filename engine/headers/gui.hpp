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
#ifdef USE_IMGUI

#include "frame_buffer.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Brenta
{

/**
 * @brief GUI class
 *
 * This class contains the GUI functions,
 * It's a wrapper around imgui, providing 
 * functions to initialize, update and delete
 * the GUI.
 */
class GUI
{
  public:
    /**
     * @brief Initialize the GUI
     */
    static void Init();
    /**
     * @brief Delete the GUI
     */
    static void Delete();
    /**
     * @brief Start a new frame
     * To be called at each frame
     * before rendering.
     */
    static void new_frame(Types::FrameBuffer *fb);
    /**
     * @brief Render the GUI
     * To be called at each frame
     * after rendering.
     */
    static void render();
};

} // namespace Brenta

#endif
