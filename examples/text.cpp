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

/**
 * Just a simple window
 */

#include "engine.hpp"

#include <iostream>

using namespace brenta;

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

int main()
{
    engine eng = engine::builder()
                     .use_screen(true)
                     .use_text(true)             /* Enable text rendering */
                     .set_text_font("arial.ttf") /* Set the font */
                     .set_text_size(48)          /* Set the font size */
                     .set_gl_blending(true)
                     .set_gl_cull_face(true)
                     .set_gl_multisample(true)
                     .set_gl_depth_test(true)
                     .set_screen_width(SCR_WIDTH)
                     .set_screen_height(SCR_HEIGHT)
                     .set_screen_is_mouse_captured(false)
                     .build();

    while (!screen::is_window_closed())
    {
        if (screen::is_key_pressed(GLFW_KEY_ESCAPE))
            screen::set_close();

        gl::set_color(0.2f, 0.3f, 0.3f, 1.0f);
        gl::clear();

        text::render_text("Hello OpenGL!", 25.0f, 25.0f, 1.0f,
                          glm::vec3(0.5f, 0.8f, 0.2));

        screen::poll_events();
        screen::swap_buffers();
    }

    return 0;
}
