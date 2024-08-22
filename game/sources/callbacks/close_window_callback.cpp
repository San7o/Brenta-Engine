#include "engine.hpp"
#include "callbacks/close_window_callback.hpp"

using namespace ECS;
using namespace ECS::Types;

void InitCloseWindowCallback() {
    auto close_window_callback = []() {
        Screen::SetClose();
    };
    Input::AddKeyboardCallback(GLFW_KEY_ESCAPE, close_window_callback);
}
