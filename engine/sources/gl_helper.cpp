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

#include "gl_helper.hpp"

#include "engine_logger.hpp"
#include "screen.hpp"
#include "text.hpp"

#include <iostream>

using namespace Brenta;
using namespace Brenta::Utils;

void GL::LoadOpenGL(bool gl_blending, bool gl_cull_face, bool gl_multisample,
                    bool gl_depth_test)
{
    GLADloadproc loadproc = (GLADloadproc) Screen::GetProcAddress();
    if (!gladLoadGLLoader(loadproc))
    {
        ERROR("Failed to initialize GLAD");
        exit(-1);
    }

    int SCR_WIDTH = Screen::GetWidth();
    int SCR_HEIGHT = Screen::GetHeight();

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); /* Set viewport */
    glEnable(GL_DEPTH_TEST);                 /* Enable depth testing */
    INFO("Enabled GL_DEPTH_TEST");

    /* Enable blending for transparency */
    if (gl_blending)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        INFO("Enabled GL_BLEND (transparency)");
    }

    /* Enable face culling, draw only visible triangles
     * based on their orientation (defined clockwise or counterclockwise) */
    if (gl_cull_face)
    {
        glEnable(GL_CULL_FACE);
        INFO("Enabled GL_CULL_FACE (draw only visible triangles)");
    }

    /* Enable multisampling
     * Only works for a multisample buffer. */
    if (gl_multisample)
    {
        glEnable(GL_MULTISAMPLE);
        INFO("Enabled GL_MULTISAMPLE");
    }

    GLenum errcode = GL::glCheckError();
    if (!errcode)
        INFO("OpenGl loaded");
}

void GL::SetPoligonMode(GLboolean enable)
{
    if (enable)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        INFO("Enabled GL_POLYGON_MODE (wireframe)");
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        INFO("Disabled GL_POLYGON_MODE (fill)");
    }
}

void GL::SetViewport(int x, int y, int SCR_WIDTH, int SCR_HEIGHT)
{
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); /* Set viewport */
}

void GL::SetColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a); /* Set clear color */
}

void GL::DrawArrays(GLenum mode, int first, int count)
{
    glDrawArrays(mode, first, count);
}

void GL::DrawElements(GLenum mode, int count, GLenum type, const void *indices)
{
    glDrawElements(mode, count, type, indices);
}

void GL::Clear()
{
    /* Clear color and depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GL::BindVertexArray(unsigned int n)
{
    glBindVertexArray(n);
}

GLenum GL::glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error = "INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION";
            break;
        case GL_STACK_OVERFLOW:
            error = "STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW:
            error = "STACK_UNDERFLOW";
            break;
        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION";
            break;
        default:
            error = "UNKNOWN";
            break;
        }

        error += " | " + std::string(file) + " (" + std::to_string(line) + ")";
        ERROR(error);
    }
    return errorCode;
}
