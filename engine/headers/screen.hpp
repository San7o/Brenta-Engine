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

// clang-format off
// The order of these includes is important
#include <glad/glad.h>
#include <GLFW/glfw3.h> /* OpenGL windowing library */
// clang-format on

namespace brenta
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
class screen
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
    static GLFWwindow *window;

    screen() = delete;
    /**
     * @brief Initialize the window
     *
     * This method initializes the window with the given
     * width and height, and sets the title of the window.
     * The window is created with the given parameters.
     *
     * @param SCR_WIDTH Width of the window
     * @param SCR_HEIGHT Height of the window
     * @param is_mouse_captured If the mouse is captured
     * @param title Title of the window
     * @param msaa If multisampling is enabled
     * @param vsync If vertical synchronization is enabled
     */
    static void init(int SCR_WIDTH, int SCR_HEIGHT,
                     bool is_mouse_captured = false,
                     const char *title = "OpenGL", bool msaa = false,
                     bool vsync = false);

    /* Getters */

    /**
     * @brief Get the width of the window
     * @return Width of the window
     */
    static int get_width();
    /**
     * @brief Get the height of the window
     */
    static int get_height();
    /**
     * @brief Check if the window is closed
     * @return If the window is closed
     */
    static bool is_window_closed();
    /**
     * @brief Check if a key is pressed
     * @param key Key to check
     * @return If the key is pressed
     */
    static bool is_key_pressed(int key);
    /**
     * @brief Get the time
     * @return Time since the start of the program
     */
    static float get_time();
    /**
     * @brief Get the window
     * @return Window
     */
    static GLFWwindow *get_window();
    /**
     * @brief Get the OpenGL function pointer
     * @return OpenGL function pointer
     */
    static GLFWglproc get_proc_address();

    /* Setters */

    /**
     * @brief Set the mouse callback
     * @param callback Callback to set
     */
    static void set_mouse_callback(GLFWcursorposfun callback);
    /**
     * @brief Set the key callback
     * @param callback Callback to set
     */
    static void set_size_callback(GLFWframebuffersizefun callback);
    /**
     * @brief Set the mouse position callback
     * @param callback Callback to set
     */
    static void set_mouse_pos_callback(GLFWcursorposfun callback);
    /**
     * @brief Set the key callback
     * @param callback Callback to set
     */
    static void set_key_callback(GLFWkeyfun callback);
    /**
     * @brief Set the mouse capture
     * @param isCaptured If the mouse is captured
     */
    static void set_mouse_capture(bool is_captured);
    /**
     * @brief Set the window close flag
     */
    static void set_close();

    /* Utils */

    /**
     * @brief Swap the front and back buffers
     *
     * Having to buffers is done to avoid flickering.
     */
    static void swap_buffers();
    /**
     * @brief Poll all pending events
     */
    static void poll_events();
    /**
     * @brief Terminate the window
     */
    static void terminate();

  private:
    static void set_context_version(int major, int minor);
    static void use_core_profile();
    static void set_hints_apple();
    static void create_window(int SCR_WIDTH, int SCR_HEIGHT, const char *title);
    static void make_context_current();
    static void framebuffer_size_callback(GLFWwindow *window, int width,
                                          int height);
};

} // namespace brenta
