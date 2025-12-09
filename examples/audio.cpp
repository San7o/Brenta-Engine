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
#include <filesystem>
#include <iostream>
#include <viotecs/viotecs.hpp>

using namespace brenta;

REGISTER_SYSTEMS()

int main()
{
  //
  // Setup
  //

  const int screen_width = 800;
  const int screen_height = 600;
  
  // Required: id and path of an audio. The id will be used by the
  // audio subsystem to identify this particular file.
  types::audio_name_t audio_guitar_id = "guitar";
  std::string audio_guitar_path = "examples/assets/audio/guitar.wav";

  // You can specify additional optional settings like stream and
  // volume
  types::stream_name_t audio_guitar_stream = "guitar_stream";
  float guitar_volume = 0.8f;
  
  auto& engine = engine::builder()
    .subsystem(logger::builder()
               .level(oak::level::debug))
    .subsystem(window::builder()
               .title("audio test")
               .width(screen_width)
               .height(screen_height))
    
    // Enable audio subsystem
    
    .subsystem(audio::builder()

               // Load a file
               
               .load(audio_guitar_id, audio_guitar_path)

               // Create a stream to play the audio (or just use "default")

               .stream(audio_guitar_stream, guitar_volume))
    
    .build();

  auto ret = engine.initialize();
  if (!ret.has_value())
  {
    ERROR("Failed to initialize subsystem {}", ret.error());
    return 1;
  }

  //
  // You can also load an audio any time with audio::load
  //
  // audio::load(audio_guitar_id, audio_guitar_path);

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
      audio::play_audio(audio_guitar_id, audio_guitar_stream);

    window::poll_events();
    window::swap_buffers();
  }

  //
  // Cleanup
  //
  
  ret = engine.terminate();
  if (!ret.has_value())
  {
    oak::error("Failed to terminate subsystem {}", ret.error());
    return 1;
  }
  return 0;
}
