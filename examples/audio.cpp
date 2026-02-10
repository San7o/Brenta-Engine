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

#include <iostream>
#include <viotecs/viotecs.hpp>

REGISTER_SYSTEMS()

using namespace brenta;
  
int main()
{
  const int screen_width = 800;
  const int screen_height = 600;
  
  // Required: id and path of an audio. The id will be used by the
  // audio subsystem to identify this particular file.
  const audio::sound_id_t sound_guitar_id = "guitar";
  const std::string sound_guitar_path = "examples/assets/audio/guitar.wav";

  // You can specify additional optional settings like stream and
  // volume
  const audio::stream_id_t stream_guitar_id = "guitar_stream";
  const float guitar_volume = 0.8f;
  
  //
  // Setup
  //

  engine::builder()
    .with(logger::builder()
          .level(oak::level::debug))
    .with(window::builder()
          .title("audio test")
          .width(screen_width)
          .height(screen_height))    
    // Enable audio subsystem
    .with(audio::builder()
          // Load a sound from path to stream
          .sound(sound_guitar_id, sound_guitar_path, stream_guitar_id)
          // Optionally set additional settings for the stream
          .stream(stream_guitar_id, guitar_volume))
    .build();

  auto engine = engine::managed();

  //
  // You can also load a sound any time with audio::load
  //
  // audio::load(sound_guitar_id, sound_guitar_path);

  //
  // Main loop
  //
  
  while (!window::should_close())
  {
    if (window::is_key_pressed(GLFW_KEY_ESCAPE))
      window::close();

    //
    // Press space to play the audio "guitar". If the stream is not
    // specified, it will be used the value "default"
    //
    if (window::is_key_pressed(GLFW_KEY_SPACE))
    {
      audio::play(sound_guitar_id);
    }

    window::poll_events();
    window::swap_buffers();
  }
  return 0;
}
