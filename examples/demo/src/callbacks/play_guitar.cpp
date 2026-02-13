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
  auto play_guitar = []() { Audio::play("guitar"); };
  Input::add_keyboard_callback(Key::P, play_guitar);
}
