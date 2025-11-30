// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

/**
 * Play a sound
 */

#include <brenta/engine.hpp>
#include <filesystem>
#include <iostream>
#include <viotecs/viotecs.hpp>

using namespace brenta;

REGISTER_SYSTEMS()

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

#define ABS(...) std::filesystem::absolute(__VA_ARGS__)

int main()
{
  engine eng = engine::builder()
                 .use_screen(true)
                 .use_logger(true)
                 .use_audio(true) // Enable audio
                 .set_screen_width(SCR_WIDTH)
                 .set_screen_height(SCR_HEIGHT)
                 .set_screen_is_mouse_captured(false)
                 .build();

  // Load an audio file, assign it the name "guitar"
  audio::load_audio("guitar", ABS("examples/assets/audio/guitar.wav"));

  while (!screen::is_window_closed())
  {
    if (screen::is_key_pressed(GLFW_KEY_ESCAPE))
      screen::set_close();

    //
    // Press space to play the audio "guitar" in the
    // default audio stream
    //
    if (screen::is_key_pressed(GLFW_KEY_SPACE))
      audio::play_audio("guitar");

    screen::poll_events();
    screen::swap_buffers();
  }

  return 0;
}
