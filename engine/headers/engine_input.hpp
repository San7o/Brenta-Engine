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

#include <functional>
#include <unordered_map>
#include <vector>
#include <GLFW/glfw3.h>
#include <string>

namespace ECS
{

namespace Types
{

typedef std::string MouseCallbackName;

} // namespace Types

class Input
{
public:
    Input() = delete;
    static void Init();

    /* Keys are defined in GLFW */
    /* https://www.glfw.org/docs/3.0/group__keys.html */
    static void AddKeyboardCallback(int key, std::function<void()> callback);
    static void RemoveKeyboardCallback(int key);
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void AddMousePosCallback(Types::MouseCallbackName name, std::function<void(double, double)> callback);
    static void RemoveMousePosCallback(Types::MouseCallbackName callback_name);
    static void MousePosCallback(GLFWwindow* window, double xpos, double ypos);
private:
    static std::unordered_map<int, std::function<void()>> keyboardCallbacks;
    static std::unordered_map<std::string, std::function<void(double, double)>> mouseCallbacks;
};

} // namespace ECS
