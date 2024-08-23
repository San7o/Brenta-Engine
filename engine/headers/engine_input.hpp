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

#include <GLFW/glfw3.h>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace Brenta
{

namespace Types
{

typedef std::string MouseCallbackName;

} // namespace Types

/**
 * @brief Input subsystem
 *
 * This subsystem is used to manage the input system of the engine. It is used
 * to add and remove callbacks for keyboard and mouse events. You can define
 * your own callbacks and register them with the input subsystem.
 */
class Input
{
  public:
    Input() = delete;
    /**
     * @brief Initialize the input system
     *
     * This function initializes the input system. It should be called before
     * any other input function is called.
     */
    static void Init();

    /**
     * @brief Add a keyboard callback
     *
     * This function adds a callback to a key press event. The callback is a
     * function that takes no arguments and returns void. The key enum is
     * defined in GLFW. When the key is pressed, the callback is called.
     *
     * @param key The key to add the callback to
     * @param callback The callback function
     */
    static void AddKeyboardCallback(int key, std::function<void()> callback);
    /**
     * @brief Remove a keyboard callback
     *
     * This function removes a callback from a key press event. The key enum is
     * defined in GLFW. When the key is pressed, the callback is removed.
     *
     * @param key The key to remove the callback from
     */
    static void RemoveKeyboardCallback(int key);
    /**
     * @brief Keyboard callback
     *
     * This function is called when a key is pressed. It calls the callback
     * function associated with the key.
     *
     * @param window The window that received the event
     * @param key The key that was pressed or released
     * @param scancode The system-specific scancode of the key
     * @param action The key action
     * @param mods Bit field describing which modifier keys were held down
     */
    static void KeyCallback(GLFWwindow *window, int key, int scancode,
                            int action, int mods);
    /**
     * @brief Add a mouse position callback
     *
     * This function adds a callback to a mouse position event. The callback is
     * a function that takes two doubles and returns void. The two doubles are
     * the x and y position of the mouse. When the mouse is moved, the callback
     * is called.
     *
     * @param name The name of the callback
     * @param callback The callback function
     */
    static void
    AddMousePosCallback(Types::MouseCallbackName name,
                        std::function<void(double, double)> callback);
    /**
     * @brief Remove a mouse position callback
     *
     * This function removes a callback from a mouse position event. When the
     * mouse is moved, the callback is removed.
     *
     * @param callback_name The name of the callback
     */
    static void RemoveMousePosCallback(Types::MouseCallbackName callback_name);
    /**
     * @brief Mouse position callback
     *
     * This function is called when the mouse is moved. It calls the callback
     * function associated with the mouse position.
     *
     * @param window The window that received the event
     * @param xpos The new x-coordinate, in screen coordinates, of the cursor
     * @param ypos The new y-coordinate, in screen coordinates, of the cursor
     */
    static void MousePosCallback(GLFWwindow *window, double xpos, double ypos);

  private:
    static std::unordered_map<int, std::function<void()>> keyboardCallbacks;
    static std::unordered_map<std::string, std::function<void(double, double)>>
        mouseCallbacks;
};

} // namespace Brenta
