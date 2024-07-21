#include "callbacks/play_guitar_callback.h"

#include "engine.h"

using namespace ECS;
using namespace ECS::Types;

void InitPlayGuitarCallback() {
    auto play_guitar = []() {
        Audio::PlayAudio("guitar");
    };
    Input::AddKeyboardCallback(GLFW_KEY_P, play_guitar);
}
