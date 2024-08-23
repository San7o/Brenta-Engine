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

#include "screen.hpp"

#include "camera.hpp"
#include "engine_audio.hpp"
#include "engine_input.hpp"
#include "engine_logger.hpp"

#include <cstdio>

using namespace Brenta;

GLFWwindow *Screen::window;
int Screen::WIDTH;
int Screen::HEIGHT;

void Screen::Init(int SCR_WIDTH, int SCR_HEIGHT, bool isMouseCaptured,
                  const char *title, bool msaa, bool vsync)
{
    Screen::WIDTH = SCR_WIDTH;
    Screen::HEIGHT = SCR_HEIGHT;

    if (glfwInit() == GLFW_FALSE)
    {
        ERROR("Failed to initialize GLFW on init");
    }

    SetContextVersion(3, 3); /* OpenGL 3.3 */
    UseCoreProfile();

    if (msaa)
    {
        glfwWindowHint(GLFW_SAMPLES, 4); /* MSAA */
        INFO("Enabled MSAA");
    }

    if (!vsync)
    {
        glfwSwapInterval(0); /* Disable VSync */
        INFO("Disabled VSync");
    }

#ifdef __APPLE__
    SetHintsApple();
#endif

    CreateWindow(SCR_WIDTH, SCR_HEIGHT, title);
    MakeContextCurrent();
    SetMouseCapture(isMouseCaptured);

    /* Set the callback for resizing the window */
    Screen::SetSizeCallback(Framebuffer_size_callback);
}

bool Screen::isWindowClosed()
{
    return glfwWindowShouldClose(Screen::window);
}

bool Screen::isKeyPressed(int key)
{
    return glfwGetKey(Screen::window, key) == GLFW_PRESS;
}

float Screen::GetTime()
{
    return glfwGetTime();
}

GLFWwindow *Screen::GetWindow()
{
    return Screen::window;
}

GLFWglproc Screen::GetProcAddress()
{
    return reinterpret_cast<void (*)()>(glfwGetProcAddress);
}

int Screen::GetWidth()
{
    return Screen::WIDTH;
}

int Screen::GetHeight()
{
    return Screen::HEIGHT;
}

void Screen::SetMouseCallback(GLFWcursorposfun callback)
{
    glfwSetCursorPosCallback(Screen::window, callback);
}

void Screen::SetSizeCallback(GLFWframebuffersizefun callback)
{
    glfwSetFramebufferSizeCallback(Screen::window, callback);

    INFO("Set framebuffer size callback");
}

void Screen::SetMouseCapture(bool isCaptured)
{
    if (isCaptured)
    {
        glfwSetInputMode(Screen::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        INFO("Mouse captured");
    }
    else
    {
        glfwSetInputMode(Screen::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        INFO("Mouse not captured");
    }
}

void Screen::SetClose()
{
    glfwSetWindowShouldClose(Screen::window, GLFW_TRUE);
}

void Screen::Terminate()
{
    glfwTerminate();
    INFO("Screen terminated");
}

void Screen::SwapBuffers()
{
    glfwSwapBuffers(Screen::window);
}

void Screen::PollEvents()
{
    glfwPollEvents();
}

void Screen::SetContextVersion(int major, int minor)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

    INFO("Set context to OpenGL version: ", major, ".", minor);
}

void Screen::SetKeyCallback(GLFWkeyfun callback)
{
    glfwSetKeyCallback(Screen::window, callback);
}

void Screen::SetMousePosCallback(GLFWcursorposfun callback)
{
    glfwSetCursorPosCallback(Screen::GetWindow(), callback);
}

void Screen::UseCoreProfile()
{
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    INFO("Set OpenGL profile to core");
}

void Screen::SetHintsApple()
{
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void Screen::CreateWindow(int SCR_WIDTH, int SCR_HEIGHT, const char *title)
{
    Screen::window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, NULL, NULL);
    if (Screen::window == NULL)
    {
        ERROR("Failed to create GLFW window");
        Terminate();
    }
}

void Screen::MakeContextCurrent()
{
    glfwMakeContextCurrent(Screen::window);
}

void Screen::Framebuffer_size_callback(GLFWwindow *window, int width,
                                       int height)
{
    glViewport(0, 0, width, height);
    Screen::WIDTH = width;
    Screen::HEIGHT = height;

    INFO("Set viewport: ", width, "x", height);
}
