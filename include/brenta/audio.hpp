// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/subsystem.hpp>

// Backend
#include <miniaudio/miniaudio.h>

#include <string>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <filesystem>

namespace brenta
{

/**
 * @brief Audio subsystem
 *
 * This class contains the audio subsystem of the engine. It is used
 * to load audio files, create audio streams, play audio files on
 * streams, set the volume, start and stop streams. The audio
 * subsystem needs to be initialized and destroyed via initialize()
 * and terminate(). Audio files and streams are stored in maps and are
 * identified by a string id.
 */
class Audio : public Subsystem
{  
public:

  using StreamId = std::string;
  using SoundId  = std::string;
  using Sound    = ma_sound;
  using Stream   = ma_sound_group;

  class      Builder;
  enum class Error;

  // This map contains all the sound files loaded by the engine.  The
  // key is the id of the sound file, the value is the struct sound_t.
  static std::unordered_map<SoundId, Sound> sounds;
  // This map contains all the audio streams created by the engine.
  // The key is the id of the stream, the value is a stream type. The
  // engine creates a default stream called "default".
  static std::unordered_map<StreamId, Stream> streams;
  
  
  // Subsystem interface
  
  static const std::string subsystem_name;
  // Initialize the audio subsystem of the engine. It creates a
  // default stream called "default".
  std::expected<void, Subsystem::Error> initialize() override;
  // Free all audio streams and sound files, and close the audio
  // subsystem.
  std::expected<void, Subsystem::Error> terminate() override;
  std::string name() override;
  bool is_initialized() override;
  
  // Constructors / destructors
  
  Audio() = default;
  ~Audio() = default;

  // Member functions
  
  static Audio &instance();

  // Automatically creates the stream if it does not exist.
  static std::expected<void, Audio::Error>
  load(const SoundId &sound_id,
       const std::filesystem::path &path,
       const StreamId &stream_id = "default");

  // Play a sound on its stream
  static std::expected<void, Audio::Error>
  play(const SoundId &id);

  //
  // Stream functions
  //
  
  static std::expected<void, Audio::Error>
  create_stream(const StreamId &id);
  static Stream *get_stream(const StreamId &id);
  
  static std::expected<void, Audio::Error>
  stream_stop(const StreamId &id);
  
  static std::expected<void, Audio::Error>
  stream_start(const StreamId &id);
  // 1.0 is default, 2.0 is double, 0.5 is half.
  static std::expected<void, Audio::Error>
  stream_set_volume(const StreamId &id, float volume);

private:
  
  // A list of pairs (stream_id, volume) of streams that will be
  // created when the subsystem is initialized.
  static std::vector<std::pair<StreamId, float>> init_streams;
  // A list of pairs (sound_id, pathname, stream_id) of sounds that
  // will be loaded when the subsystem is initialized.
  static std::vector<std::tuple<SoundId,
                                std::filesystem::path,
                                StreamId>> init_sounds;
  static bool initialized;
  
  // Backend
  static ma_engine engine;
};

class Audio::Builder : public Subsystem::Builder
{
public:
  
  Builder() = default;
  ~Builder() = default;

  Builder &sound(const SoundId &sound_id,
                 const std::filesystem::path &path,
                 const StreamId &stream_id = "default");
  Builder &stream(const StreamId &id,
                  float volume = 1.0);
  
  brenta::Subsystem &build() override;
  
private:
  
  std::vector<std::tuple<SoundId, std::filesystem::path,
                         StreamId>> init_sounds;
  std::vector<std::pair<StreamId, float>> init_streams;
  float volume = 1.0;
};

enum class Audio::Error : int
{
  InitFromFile,
  StreamNotFound,
  SoundNotFound,
  StreamInit,
  StreamStop,
  StreamStart,
};
  
} // namespace brenta
