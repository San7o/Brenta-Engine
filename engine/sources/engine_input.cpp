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

using namespace Brenta;
using namespace Brenta::Types;
using namespace Brenta::Utils;

std::unordered_map<int, std::function<void()>> Input::keyboardCallbacks;
std::unordered_map<std::string, std::function<void(double, double)>>
    Input::mouseCallbacks;

void Input::Init()
{
    Screen::SetKeyCallback(Input::KeyCallback);
    Screen::SetMousePosCallback(Input::MousePosCallback);

    INFO("Input initialized");
}

void Input::AddKeyboardCallback(int key, std::function<void()> callback)
{
    Input::keyboardCallbacks[key] = callback;
    INFO("Added callback for key: " + std::to_string(key));
}

void Input::RemoveKeyboardCallback(int key)
{
    if (Input::keyboardCallbacks.find(key) == Input::keyboardCallbacks.end())
    {
        ERROR("No callback found for key: ", key);
        return;
    }

    Input::keyboardCallbacks.erase(key);
    INFO("Removed callback for key: ", key);
}

void Input::KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                        int mods)
{
    if (action == GLFW_PRESS)
    {
        if (Input::keyboardCallbacks.find(key)
            != Input::keyboardCallbacks.end())
        {
            Input::keyboardCallbacks.at(key)();
        }
    }
}

void Input::AddMousePosCallback(MouseCallbackName callback_name,
                                std::function<void(double, double)> callback)
{
    Input::mouseCallbacks[callback_name] = callback;
    INFO("Added callback for mouse: ", callback_name);
}

void Input::RemoveMousePosCallback(MouseCallbackName callback_name)
{
    if (Input::mouseCallbacks.find(callback_name)
        == Input::mouseCallbacks.end())
    {
        ERROR("No callback found for mouse: ", callback_name);
        return;
    }

    Input::mouseCallbacks.erase(callback_name);
    INFO("Removed callback for mouse: ", callback_name);
}

void Input::MousePosCallback(GLFWwindow *window, double xpos, double ypos)
{
    for (auto &callback : Input::mouseCallbacks)
    {
        callback.second(xpos, ypos);
    }
}
