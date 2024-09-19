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

using namespace brenta;

GLFWwindow *screen::window;
int screen::WIDTH;
int screen::HEIGHT;

void screen::init(int SCR_WIDTH, int SCR_HEIGHT, bool is_mouse_captured,
                  const char *title, bool msaa, bool vsync)
{
    screen::WIDTH = SCR_WIDTH;
    screen::HEIGHT = SCR_HEIGHT;

    if (glfwInit() == GLFW_FALSE)
    {
        ERROR("Failed to initialize GLFW on init");
    }

    set_context_version(3, 3); /* OpenGL 3.3 */
    use_core_profile();

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
    set_hints_apple();
#endif

    create_window(SCR_WIDTH, SCR_HEIGHT, title);
    make_context_current();
    set_mouse_capture(is_mouse_captured);

    /* Set the callback for resizing the window */
    screen::set_size_callback(framebuffer_size_callback);
}

bool screen::is_window_closed()
{
    return glfwWindowShouldClose(screen::window);
}

bool screen::is_key_pressed(int key)
{
    return glfwGetKey(screen::window, key) == GLFW_PRESS;
}

float screen::get_time()
{
    return glfwGetTime();
}

GLFWwindow *screen::get_window()
{
    return screen::window;
}

GLFWglproc screen::get_proc_address()
{
    return reinterpret_cast<void (*)()>(glfwGetProcAddress);
}

int screen::get_width()
{
    return screen::WIDTH;
}

int screen::get_height()
{
    return screen::HEIGHT;
}

void screen::set_mouse_callback(GLFWcursorposfun callback)
{
    glfwSetCursorPosCallback(screen::window, callback);
}

void screen::set_size_callback(GLFWframebuffersizefun callback)
{
    glfwSetFramebufferSizeCallback(screen::window, callback);

    INFO("Set framebuffer size callback");
}

void screen::set_mouse_capture(bool is_captured)
{
    if (is_captured)
    {
        glfwSetInputMode(screen::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        INFO("Mouse captured");
    }
    else
    {
        glfwSetInputMode(screen::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        INFO("Mouse not captured");
    }
}

void screen::set_close()
{
    glfwSetWindowShouldClose(screen::window, GLFW_TRUE);
}

void screen::terminate()
{
    INFO("Terminating screen");
    glfwDestroyWindow(screen::window);
    glfwTerminate();
    INFO("screen terminated");
}

void screen::swap_buffers()
{
    glfwSwapBuffers(screen::window);
}

void screen::poll_events()
{
    glfwPollEvents();
}

void screen::set_context_version(int major, int minor)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

    INFO("Set context to OpenGL version: {}.{}", major, minor);
}

void screen::set_key_callback(GLFWkeyfun callback)
{
    glfwSetKeyCallback(screen::window, callback);
}

void screen::set_mouse_pos_callback(GLFWcursorposfun callback)
{
    glfwSetCursorPosCallback(screen::get_window(), callback);
}

void screen::use_core_profile()
{
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    INFO("Set OpenGL profile to core");
}

void screen::set_hints_apple()
{
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void screen::create_window(int SCR_WIDTH, int SCR_HEIGHT, const char *title)
{
    screen::window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, NULL, NULL);
    if (screen::window == NULL)
    {
        ERROR("Failed to create GLFW window");
        terminate();
    }
}

void screen::make_context_current()
{
    glfwMakeContextCurrent(screen::window);
}

void screen::framebuffer_size_callback(GLFWwindow *window, int width,
                                       int height)
{
#ifndef USE_IMGUI
    glViewport(0, 0, width, height);
    screen::WIDTH = width;
    screen::HEIGHT = height;
#endif
}
