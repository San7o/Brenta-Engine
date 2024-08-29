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

using namespace Brenta;
using namespace Brenta::Types;

const int SCR_WIDTH = 1280;
const int SCR_HEIGHT = 720;
const bool isMouseCaptured = false;

int main()
{
    Engine engine = Engine::Builder()
                        .use_screen(true)
                        .set_screen_width(SCR_WIDTH)
                        .set_screen_height(SCR_HEIGHT)
                        .set_screen_is_mouse_captured(isMouseCaptured)
                        .build();


    // A square
    float vertices[] = {
        // First Triangle
        -1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,

        // Second Triangle
        -1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f
    };
    VAO vao;
    vao.Init();
    Buffer vbo = Buffer(GL_ARRAY_BUFFER);
    vbo.CopyData(sizeof(vertices), vertices, GL_STATIC_DRAW);
    vao.SetVertexData(vbo, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    Shader::New("fractal", 
                 GL_VERTEX_SHADER,
                 "examples/mandelbrot.vs",
                 GL_FRAGMENT_SHADER,
                 "examples/mandelbrot.fs");
#ifdef USE_IMGUI
    Brenta::Types::FrameBuffer fb(SCR_WIDTH, SCR_HEIGHT);
#endif
    float zoom = 1.0f;
    glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f);

    while (!Screen::isWindowClosed())
    {
        Screen::PollEvents();
        if (Screen::isKeyPressed(GLFW_KEY_ESCAPE))
            Screen::SetClose();
        // Use arrows to move the fractal
        if (Screen::isKeyPressed(GLFW_KEY_UP))
            zoom += 0.01f;
        if (Screen::isKeyPressed(GLFW_KEY_DOWN))
            zoom -= 0.01f;
        if (Screen::isKeyPressed(GLFW_KEY_LEFT))
            offset.x -= 0.005f;
        if (Screen::isKeyPressed(GLFW_KEY_RIGHT))
            offset.x += 0.005f;
        if (Screen::isKeyPressed(GLFW_KEY_W))
            offset.y += 0.005f;
        if (Screen::isKeyPressed(GLFW_KEY_S))
            offset.y -= 0.005f;

#ifdef USE_IMGUI
        GUI::new_frame(&fb);
#endif
        fb.Bind();
        GL::Clear();

        // Render here
        vao.Bind();
        Shader::Use("fractal");
        Shader::SetInt("fractal", "texture", fb.texture_id);
        Shader::SetVec3("fractal", "resolution", glm::vec3(float(SCR_WIDTH), float(SCR_HEIGHT), 0.0f));
        Shader::SetVec3("fractal", "offset", offset);
        Shader::SetFloat("fractal", "zoom", zoom);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glCheckError();

        vao.Unbind();
        fb.Unbind();
#ifdef USE_IMGUI
        GUI::render();
#endif

        Screen::SwapBuffers();
    }

    return 0;
}
