// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/subsystem.hpp>

#include <tenno/vector.hpp>
#include <tenno/memory.hpp>

#include <string>
#include <tuple>
#include <filesystem>

namespace brenta
{

class AudioDriver;

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

  using StreamId     = std::string;
  using StreamHandle = void*;
  using SoundId      = std::string;
  
  class      Builder;
  enum class Error;

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
  
  static std::expected<void, Audio::Error>
  stream_stop(const StreamId &id);
  
  static std::expected<void, Audio::Error>
  stream_start(const StreamId &id);
  // 1.0 is default, 2.0 is double, 0.5 is half.
  static std::expected<void, Audio::Error>
  stream_set_volume(const StreamId &id, float volume);
  
  static std::optional<Audio::StreamHandle>
  get_stream(const StreamId &stream_id);
  
private:
  
  // A list of pairs (stream_id, volume) of streams that will be
  // created when the subsystem is initialized.
  static tenno::vector<std::pair<StreamId, float>> init_streams;
  // A list of pairs (sound_id, pathname, stream_id) of sounds that
  // will be loaded when the subsystem is initialized.
  static tenno::vector<std::tuple<SoundId,
                                  std::filesystem::path,
                                  StreamId>> init_sounds;
  static bool initialized;

  static tenno::shared_ptr<AudioDriver> backend;

  // Private constructors / destructors for singleton
  Audio()  = default;
  ~Audio() { this->terminate(); }

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
  
  tenno::vector<std::tuple<SoundId, std::filesystem::path,
                           StreamId>> init_sounds;
  tenno::vector<std::pair<StreamId, float>> init_streams;
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
  Uninitialized,
};

class AudioDriver
{
public:

  virtual ~AudioDriver() {}

  virtual std::expected<void, std::string> initialize() = 0;
  virtual std::expected<void, std::string> terminate() = 0;

  // Automatically creates the stream if it does not exist.
  virtual std::expected<void, Audio::Error>
  load(const Audio::SoundId &sound_id,
       const std::filesystem::path &path,
       const Audio::StreamId &stream_id = "default") = 0;

  // Play a sound on its stream
  virtual std::expected<void, Audio::Error>
  play(const Audio::SoundId &id) = 0;

  //
  // Stream functions
  //
  
  virtual std::expected<void, Audio::Error>
  create_stream(const Audio::StreamId &id) = 0;
  
  virtual std::expected<void, Audio::Error>
  stream_stop(const Audio::StreamId &id) = 0;
  
  virtual std::expected<void, Audio::Error>
  stream_start(const Audio::StreamId &id) = 0;
  
  // 1.0 is default, 2.0 is double, 0.5 is half.
  virtual std::expected<void, Audio::Error>
  stream_set_volume(const Audio::StreamId &id, float volume) = 0;

  virtual std::optional<Audio::StreamHandle>
  get_stream(const Audio::StreamId &stream_id) = 0;
  
};
  
} // namespace brenta
