// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/audio.hpp>
#include <brenta/input.hpp>
#include <demo/callbacks/play_guitar.hpp>

using namespace brenta;

void init_play_guitar_callback()
{
  auto play_guitar = []() { audio::play("guitar"); };
  input::add_keyboard_callback(GLFW_KEY_P, play_guitar);
}
