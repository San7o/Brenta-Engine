// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

//
// audio.cpp
// =========
//
// Play a sound with the spacebar
//

#include <brenta/engine.hpp>
#include <brenta/audio.hpp>
#include <brenta/window.hpp>
#include <brenta/logger.hpp>

#include <viotecs/viotecs.hpp>
REGISTER_SYSTEMS()

#include <iostream>

using namespace brenta;
  
int main()
{
  const int screen_width = 800;
  const int screen_height = 600;
  
  // Required: id and path of an audio. The id will be used by the
  // audio subsystem to identify this particular file.
  const auto sound_guitar_id = "guitar";
  const auto sound_guitar_path = "examples/assets/audio/guitar.wav";

  // You can specify additional optional settings like stream and
  // volume
  const auto stream_guitar_id = "guitar_stream";
  const auto guitar_volume = 0.8f;
  
  //
  // Setup
  //

  Engine::Builder()
    .with(Logger::Builder()
          .level(oak::level::debug))
    .with(Window::Builder()
          .title("audio test")
          .width(screen_width)
          .height(screen_height))    
    // Enable audio subsystem
    .with(Audio::Builder()
          // Load a sound from path to stream
          .sound(sound_guitar_id, sound_guitar_path, stream_guitar_id)
          // Optionally set additional settings for the stream
          .stream(stream_guitar_id, guitar_volume))
    .build();
  auto engine = Engine::managed();

  //
  // You can also load a sound any time with Audio::load
  //
  // Audio::load(sound_guitar_id, sound_guitar_path);

  //
  // Main loop
  //
  
  while (!Window::should_close())
  {
    if (Window::is_key_pressed(Key::Escape))
      Window::close();

    //
    // Press space to play the audio "guitar". If the stream is not
    // specified, it will be used the value "default"
    //
    if (Window::is_key_pressed(Key::Space))
    {
      Audio::play(sound_guitar_id);
    }

    Window::poll_events();
    Window::swap_buffers();
  }
  return 0;
}
