#include "callbacks/play_guitar_callback.hpp"
#include "engine.hpp"

using namespace Brenta;

void InitPlayGuitarCallback() {
    auto play_guitar = []() {
        Audio::PlayAudio("guitar");
    };
    Input::AddKeyboardCallback(GLFW_KEY_P, play_guitar);
}
