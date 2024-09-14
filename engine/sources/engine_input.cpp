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

#include "engine.hpp"

using namespace brenta;
using namespace brenta::types;

std::unordered_map<int, std::function<void()>> input::keyboard_callbacks;
std::unordered_map<std::string, std::function<void(double, double)>>
    input::mouse_callbacks;

void input::init()
{
    screen::set_key_callback(input::key_callback);
    screen::set_mouse_pos_callback(input::mouse_pos_callback);

    INFO("Input initialized");
}

void input::add_keyboard_callback(int key, std::function<void()> callback)
{
    input::keyboard_callbacks[key] = callback;
    INFO("Added callback for key: " + std::to_string(key));
}

void input::remove_keyboard_callback(int key)
{
    if (input::keyboard_callbacks.find(key) == input::keyboard_callbacks.end())
    {
        ERROR("No callback found for key: ", key);
        return;
    }

    input::keyboard_callbacks.erase(key);
    INFO("Removed callback for key: ", key);
}

void input::key_callback(GLFWwindow *window, int key, int scancode, int action,
                        int mods)
{
    if (action == GLFW_PRESS)
    {
        if (input::keyboard_callbacks.find(key)
            != input::keyboard_callbacks.end())
        {
            input::keyboard_callbacks.at(key)();
        }
    }
}

void input::add_mouse_pos_callback(mouse_callback_name_t callback_name,
                                std::function<void(double, double)> callback)
{
    input::mouse_callbacks[callback_name] = callback;
    INFO("Added callback for mouse: ", callback_name);
}

void input::remove_mouse_pos_callback(mouse_callback_name_t callback_name)
{
    if (input::mouse_callbacks.find(callback_name)
        == input::mouse_callbacks.end())
    {
        ERROR("No callback found for mouse: ", callback_name);
        return;
    }

    input::mouse_callbacks.erase(callback_name);
    INFO("Removed callback for mouse: ", callback_name);
}

void input::mouse_pos_callback(GLFWwindow *window, double xpos, double ypos)
{
    for (auto &callback : input::mouse_callbacks)
    {
        callback.second(xpos, ypos);
    }
}
