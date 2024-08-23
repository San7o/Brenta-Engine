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

namespace Brenta
{

/**
 * @brief Screen subsystem
 *
 * This class is used to create a window and handle
 * all the events related to the window. This class
 * provides methods to initialize the window, get the
 * window size, check if a key is pressed, get the time
 * since the start of the program, and more.
 */
class Screen
{
public:
    /**
     * @brief Width of the window
     */
    static int WIDTH;
    /**
     * @brief Height of the window
     */
    static int HEIGHT;
    /**
     * @brief Pointer to the window
     */
    static GLFWwindow* window;

    Screen() = delete;
    /**
     * @brief Initialize the window
     *
     * This method initializes the window with the given
     * width and height, and sets the title of the window.
     * The window is created with the given parameters.
     *
     * @param SCR_WIDTH Width of the window
     * @param SCR_HEIGHT Height of the window
     * @param isMouseCaptured If the mouse is captured
     * @param title Title of the window
     * @param msaa If multisampling is enabled
     * @param vsync If vertical synchronization is enabled
     */
    static void Init(int SCR_WIDTH, int SCR_HEIGHT,
                     bool isMouseCaptured = false,
                     const char* title = "OpenGL",
                     bool msaa = false, bool vsync = false);

    /* Getters */

    /**
     * @brief Get the width of the window
     * @return Width of the window
     */
    static int   GetWidth();
    /**
     * @brief Get the height of the window
     */
    static int   GetHeight();
    /**
     * @brief Check if the window is closed
     * @return If the window is closed
     */
    static bool  isWindowClosed();
    /**
     * @brief Check if a key is pressed
     * @param key Key to check
     * @return If the key is pressed
     */
    static bool  isKeyPressed(int key);
    /**
     * @brief Get the time
     * @return Time since the start of the program
     */
    static float GetTime();
    /**
     * @brief Get the window
     * @return Window
     */
    static GLFWwindow* GetWindow();
    /**
     * @brief Get the OpenGL function pointer
     * @return OpenGL function pointer
     */
    static GLFWglproc GetProcAddress();

    /* Setters */

    /**
     * @brief Set the mouse callback
     * @param callback Callback to set
     */
    static void SetMouseCallback(GLFWcursorposfun callback);
    /**
     * @brief Set the key callback
     * @param callback Callback to set
     */
    static void SetSizeCallback(GLFWframebuffersizefun callback);
    /**
     * @brief Set the mouse position callback
     * @param callback Callback to set
     */
    static void SetMousePosCallback(GLFWcursorposfun callback);
    /**
     * @brief Set the key callback
     * @param callback Callback to set
     */
    static void SetKeyCallback(GLFWkeyfun callback);
    /**
     * @brief Set the mouse capture
     * @param isCaptured If the mouse is captured
     */
    static void SetMouseCapture(bool isCaptured);
    /**
     * @brief Set the window close flag
     */
    static void SetClose();

    /* Utils */

    /**
     * @brief Swap the front and back buffers
     *
     * Having to buffers is done to avoid flickering.
     */
    static void SwapBuffers();
    /**
     * @brief Poll all pending events
     */
    static void PollEvents();
    /**
     * @brief Terminate the window
     */
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

} // namespace Brenta
