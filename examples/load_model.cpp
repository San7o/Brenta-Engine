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
 * Load a model and render it on the screen.
 */

#include "engine.hpp"

#include <filesystem>
#include <iostream>

using namespace brenta;

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

#define ABS(...) std::filesystem::absolute(__VA_ARGS__)

int main()
{
    engine eng = engine::builder()
                        .use_screen(true)
                        .set_screen_width(SCR_WIDTH)
                        .set_screen_height(SCR_HEIGHT)
                        .set_screen_is_mouse_captured(false)
                        .set_gl_blending(true)
                        .set_gl_cull_face(true)
                        .set_gl_multisample(true)
                        .set_gl_depth_test(true)
                        .set_log_level(types::log_level::DEBUG)
                        .build();

    /* Load the model */
    model our_model(ABS("assets/models/backpack/backpack.obj"));

    /* Load the shader */
    shader::create("default_shader", GL_VERTEX_SHADER,
                ABS("examples/default_shader.vs"), GL_FRAGMENT_SHADER,
                ABS("examples/default_shader.fs"));

    while (!screen::is_window_closed())
    {
        /* Input */
        if (screen::is_key_pressed(GLFW_KEY_ESCAPE))
            screen::set_close();

        /* Clear */
        gl::set_color(0.2f, 0.2f, 0.207f, 1.0f);
        gl::clear();

        /* Draw */
        shader::use("default_shader");

        /* Make transformations */
        glm::mat4 view = glm::mat4(1.0f); /* Camera position */
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f,
            100.0f);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));

        shader::set_mat4("default_shader", "view", view);
        shader::set_mat4("default_shader", "projection", projection);
        shader::set_mat4("default_shader", "model", model);

        /* Draw the model */
        our_model.draw("default_shader");

        screen::poll_events();
        screen::swap_buffers();
    }

    return 0;
}
