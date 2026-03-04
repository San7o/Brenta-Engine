// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

//
// sound.cpp
// =========
//
// Play a sound with the spacebar
//

#include <brenta/engine.hpp>
#include <brenta/sound.hpp>
#include <brenta/window.hpp>
#include <brenta/input.hpp>
#include <brenta/logger.hpp>

using namespace brenta;
  
int main()
{
  Engine::Builder()
    .with(Logger::Builder()
          .level(Logger::Level::Debug))
    .with(Window::Builder()
          .title("audio test")
          .width(800)
          .height(600))
    // Enable sound subsystem
    .with(SoundManager::Builder())
    .build();
  auto engine = Engine::managed();

  auto guitar_sound_asset = SoundAsset::Builder()
    .path("examples/assets/audio/guitar.wav")
    .build();
  if (!guitar_sound_asset)
  {
    ERROR("Error loading guitar sound");
    return 1;
  }

  auto guitar_sound  = Sound(guitar_sound_asset.value());
  auto guitar_sound2 = Sound(guitar_sound_asset.value());

  //
  // Main loop
  //

  //
  // Press space or A to play a sound
  //
  Input::add_keyboard_callback(Key::Space, [&guitar_sound]() {
    guitar_sound.play();
  });
  Input::add_keyboard_callback(Key::A, [&guitar_sound2]() {
    guitar_sound2.play();
  });
  
  while (!Window::should_close())
  {
    if (Window::is_key_pressed(Key::Escape))
      Window::close();

    Window::poll_events();
    Window::swap_buffers();
  }
  return 0;
}
