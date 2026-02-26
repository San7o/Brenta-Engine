// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/audio.hpp>

#include <miniaudio/miniaudio.h>

#include <unordered_map>

namespace brenta
{

class Miniaudio : public AudioDriver
{
public:

  using Sound    = ma_sound;
  using Stream   = ma_sound_group;

  Miniaudio()  = default;
  ~Miniaudio() = default;

  std::expected<void, std::string> initialize() override;
  std::expected<void, std::string> terminate() override;

  // Automatically creates the stream if it does not exist.
  std::expected<void, Audio::Error>
  load(const Audio::SoundId &sound_id,
       const std::filesystem::path &path,
       const Audio::StreamId &stream_id = "default") override;

  // Play a sound on its stream
  std::expected<void, Audio::Error>
  play(const Audio::SoundId &id) override;

  //
  // Stream functions
  //
  
  std::expected<void, Audio::Error>
  create_stream(const Audio::StreamId &id) override;
  
  std::expected<void, Audio::Error>
  stream_stop(const Audio::StreamId &id) override;
  
  std::expected<void, Audio::Error>
  stream_start(const Audio::StreamId &id) override;
  
  std::expected<void, Audio::Error>
  stream_set_volume(const Audio::StreamId &id, float volume) override;

  std::optional<Audio::StreamHandle>
  get_stream(const Audio::StreamId &stream_id) override;
  
private:

  // This map contains all the sound files loaded by the engine.  The
  // key is the id of the sound file, the value is the struct sound_t.
  std::unordered_map<Audio::SoundId, Sound>   sounds;
  // This map contains all the audio streams created by the engine.
  // The key is the id of the stream, the value is a stream type. The
  // engine creates a default stream called "default".
  std::unordered_map<Audio::StreamId, Stream> streams;
  
  ma_engine engine;
};
  
} // namespace brenta
