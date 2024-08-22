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
 * The above copyright notice and this permission notice shall be included in all
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

#include <glad/glad.h>       
#include <GLFW/glfw3.h>      /* OpenGL windowing library */

namespace ECS
{

class Screen
{
public:
    static int WIDTH;
    static int HEIGHT;
    static GLFWwindow* window;

    Screen() = delete;
    static void Init(int SCR_WIDTH, int SCR_HEIGHT,
                     bool isMouseCaptured = false,
                     const char* title = "OpenGL");

    static int   GetWidth();
    static int   GetHeight();
    static bool  isWindowClosed();
    static bool  isKeyPressed(int key);
    static float GetTime();
    static GLFWwindow* GetWindow();
    static GLFWglproc GetProcAddress();

    static void SetMouseCallback(GLFWcursorposfun callback);
    static void SetSizeCallback(GLFWframebuffersizefun callback);
    static void SetMousePosCallback(GLFWcursorposfun callback);
    static void SetKeyCallback(GLFWkeyfun callback);
    static void SetMouseCapture(bool isCaptured);
    static void SetClose();

    /* Swap the front and back buffers to avoid flickering */
    static void SwapBuffers();
    /* Process all pending events */
    static void PollEvents();
    static void Terminate();

private:
    static void SetContextVersion(int major, int minor);
    static void UseCoreProfile();
    static void SetHintsApple();
    static void CreateWindow(int SCR_WIDTH, int SCR_HEIGHT, const char* title);
    static void MakeContextCurrent();
    static void Framebuffer_size_callback(GLFWwindow* window,
                                          int width, int height);
};

} // namespace ECS
