#pragma once

#ifndef ENGINE_INPUT_H
#define ENGINE_INPUT_H

#include <functional>
#include <unordered_map>

namespace ECS {

class Input {
public:
    Input() = delete;
    static void Init();

    /* Keys are defined in GLFW */
    /* https://www.glfw.org/docs/3.0/group__keys.html */
    static void AddCallback(int key, std::function<void()> callback);
    static void RemoveCallback(int key);
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
private:
    static std::unordered_map<int, std::function<void()>> callbacks;
};

} // namespace ECS

#endif
