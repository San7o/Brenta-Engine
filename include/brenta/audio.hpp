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

namespace brenta
{

namespace types
{

typedef std::string stream_id_t;
typedef std::string sound_id_t;
typedef ma_sound sound_t;
typedef ma_sound_group stream_t;

} // namespace types

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
class audio : public subsystem
{  
public:
  
  /**
   * @brief Map of sound files
   *
   * This map contains all the sound files loaded by the engine.  The
   * key is the id of the sound file, the value is the struct sound_t.
   */
  static std::unordered_map<types::sound_id_t, types::sound_t> sounds;
  
  /**
   * @brief Map of audio streams
   *
   * This map contains all the audio streams created by the engine.
   * The key is the id of the stream, the value is a stream type. The
   * engine creates a default stream called "default".
   */
  static std::unordered_map<types::stream_id_t, types::stream_t> streams;
  
  static const std::string subsystem_name;
  
  class builder;
  enum class error;

  // Subsystem interface
  /**
   * @brief Initialize the audio subsystem
   *
   * This function initializes the audio subsystem of the engine. It
   * creates a default stream called "default".
   */
  std::expected<void, subsystem::error> initialize() override;
    
  /**
   * @brief Terminate the audio system
   *
   * This function frees all audio streams and sound files, and closes
   * the audio subsystem.
   */
  std::expected<void, subsystem::error> terminate() override;

  std::string name() override;
  bool is_initialized() override;
  
  // Constructors / destructors
  
  audio() = default;
  ~audio() = default;

  // Member functions
  
  static audio &instance();

  /**
   * @brief Load a sound from path on a stream
   *
   * Automatically creates the stream if it does not exist.
   */
  static std::expected<void, audio::error>
  load(const types::sound_id_t &sound_id,
       const std::string &path,
       const types::stream_id_t &stream_id = "default");

  /**
   * @brief Play a sound on its stream
   */
  static std::expected<void, audio::error>
  play(const types::sound_id_t &id);

  //
  // Stream functions
  //
  
  static std::expected<void, audio::error>
  create_stream(const types::stream_id_t &id);
  static types::stream_t *get_stream(const types::stream_id_t &id);
  
  static std::expected<void, audio::error>
  stream_stop(const types::stream_id_t &id);
  
  static std::expected<void, audio::error>
  stream_start(const types::stream_id_t &id);
  /**
   * @brief Set the volume of a stream.
   *
   * 1.0 is default, 2.0 is double, 0.5 is half.
   */
  static std::expected<void, audio::error>
  stream_set_volume(const types::stream_id_t &id, float volume);

private:
  
  /**
   * A list of pairs (stream_id, volume) of streams that will be
   * created when the subsystem is initialized.
   */  
  static std::vector<std::pair<types::stream_id_t, float>> init_streams;

  /**
   * A list of pairs (sound_id, pathname, stream_id) of sounds that
   * will be loaded when the subsystem is initialized.
   */
  static std::vector<std::tuple<types::sound_id_t,
                                std::string,
                                types::stream_id_t>> init_sounds;

  static bool initialized;
  
  // Backend
  static ma_engine engine;

};

class audio::builder : public subsystem::builder
{
private:
  
  std::vector<std::tuple<types::sound_id_t, std::string,
                        types::stream_id_t>> init_sounds;
  std::vector<std::pair<types::stream_id_t, float>> init_streams;
  float volume = 1.0;
  
public:

  builder() = default;
  ~builder() = default;

  builder &sound(const types::sound_id_t &sound_id,
                 const std::string &path,
                 const types::stream_id_t &stream_id = "default");
  builder &stream(const types::stream_id_t &id,
                  float volume = 1.0);
  
  brenta::subsystem &build() override;
  
};

enum class audio::error : int
{
  init_from_file,
  stream_not_found,
  sound_not_found,
  stream_init,
  stream_stop,
  stream_start,
};
  
} // namespace brenta
