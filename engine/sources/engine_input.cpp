#include "engine.h"

using namespace ECS;
using namespace ECS::Types;

std::unordered_map<int, std::function<void()>> Input::callbacks;

void Input::Init() {
    Screen::SetKeyCallback(Input::KeyCallback);
}

void Input::AddCallback(int key, std::function<void()> callback) {
    Input::callbacks[key] = callback;
}

void Input::RemoveCallback(int key) {
    Input::callbacks.erase(key);
}

void Input::KeyCallback(GLFWwindow* window, int key,
                int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (Input::callbacks.find(key) != Input::callbacks.end()) {
            Input::callbacks.at(key)();
        }
    }
}
