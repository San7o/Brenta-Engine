#include "engine.hpp"
#include "callbacks/close_window_callback.hpp"

using namespace Brenta;

void InitCloseWindowCallback() {
    auto close_window_callback = []() {
        Screen::SetClose();
    };
    Input::AddKeyboardCallback(GLFW_KEY_ESCAPE, close_window_callback);
}
