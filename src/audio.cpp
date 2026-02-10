// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/audio.hpp>
#include <brenta/logger.hpp>

using namespace brenta;

//
// Static variables
//

std::vector<std::tuple<audio::sound_id_t, std::string,
                       audio::stream_id_t>> audio::init_sounds;
std::vector<std::pair<audio::stream_id_t, float>> audio::init_streams;
std::unordered_map<audio::sound_id_t, audio::sound_t> audio::sounds;
std::unordered_map<audio::stream_id_t, audio::stream_t> audio::streams;
ma_engine audio::engine;
bool audio::initialized = false;
const std::string audio::subsystem_name = "audio";

//
// Subsystem interface
//

std::expected<void, subsystem::error> audio::initialize()
{
  if (this->is_initialized()) return {};
  ma_result result;
  result = ma_engine_init(NULL, &audio::engine);
  if (result != MA_SUCCESS)
  {
    ERROR("{}: error initializing engine: {}",
          audio::subsystem_name, ma_result_description(result));
    return std::unexpected("Initializing audio backend");
  }

  for (auto& f : audio::init_sounds)
  {
    if (!audio::load(std::get<0>(f), std::get<1>(f), std::get<2>(f)).has_value())
      return std::unexpected("Loading audio " + get<0>(f));
  }

  for (auto& s : audio::init_streams)
  {
    if (!audio::create_stream(s.first).has_value())
      return std::unexpected("Creating stream " + s.first);
    if (!audio::stream_set_volume(s.first, s.second).has_value())
      return std::unexpected("Setting volume for stream " + s.first);
  }

  if (!audio::get_stream("default"))
  {
    if (!audio::create_stream("default").has_value())
      return std::unexpected("Creating stream default");
  }

  audio::initialized = true;
  INFO("{}: initialized", audio::subsystem_name);
  return {};
}

std::expected<void, subsystem::error> audio::terminate()
{
  if (!this->is_initialized()) return {};
  
  for (auto sound : audio::sounds)
    ma_sound_uninit(&sound.second);
  
  for (auto stream : audio::streams)
    ma_sound_group_uninit(&stream.second);

  ma_engine_uninit(&audio::engine);

  audio::initialized = false;
  INFO("{}: termianted", audio::subsystem_name);
  return {};
}

std::string audio::name()
{
  return audio::subsystem_name;
}

bool audio::is_initialized()
{
  return audio::initialized;
}

//
// Member functions
//

audio &audio::instance()
{
  static audio _audio;
  return _audio;
}

std::expected<void, audio::error>
audio::load(const audio::sound_id_t &sound_id,
            const std::string &path,
            const audio::stream_id_t &stream_id)
{
  audio::stream_t *stream = audio::get_stream(stream_id);
  if (!stream)
  {
    audio::create_stream(stream_id);
    stream = audio::get_stream(stream_id);
    if (!stream)
    {
      ERROR("{}: error stream {} not found",
            audio::subsystem_name, stream_id);
      return std::unexpected(audio::error::stream_not_found);
    }
  }

  audio::sound_t sound = {};
  audio::sounds.insert({sound_id, sound});
  if (ma_sound_init_from_file(&audio::engine, path.c_str(), 0, stream, NULL,
                              &audio::sounds.at(sound_id)) != MA_SUCCESS)
  {
    ERROR("{}: error loading sound {} from path {}",
          audio::subsystem_name, sound_id, path);
    return std::unexpected(audio::error::init_from_file);
  }
  
  INFO("{}: loaded sound {} from {} in stream {}",
       audio::subsystem_name, sound_id, path, stream_id);
  return {};
}

std::expected<void, audio::error>
audio::play(const audio::sound_id_t &id)
{
  audio::sound_t *sound = &audio::sounds.at(id);
  if (!sound)
  {
    ERROR("{}: sound with id {} not found",
          audio::subsystem_name, id);
    return std::unexpected(audio::error::sound_not_found);
  }
  ma_sound_start(sound);
  return {};
}

std::expected<void, audio::error>
audio::create_stream(const audio::stream_id_t &id)
{
  audio::stream_t *stream = audio::get_stream(id);
  if (stream) return {};

  audio::stream_t s = {};
  audio::streams.insert({id, s});
  stream = &audio::streams.at(id);
  if (ma_sound_group_init(&audio::engine, 0, NULL, stream)
      != MA_SUCCESS)
  {
    ERROR("{}: error creating audio stream {}",
          audio::subsystem_name, id);
    return std::unexpected(audio::error::stream_init);
  }
  
  INFO("{}: stream {} created", audio::subsystem_name, id);
  return {};
}

audio::stream_t *audio::get_stream(const audio::stream_id_t &id)
{
  if (audio::streams.find(id) == audio::streams.end())
  {
    return nullptr;
  }
  return &audio::streams.at(id);
}

std::expected<void, audio::error>
audio::stream_set_volume(const audio::stream_id_t &id, float volume)
{
  audio::stream_t *stream = audio::get_stream(id);
  if (!stream)
  {
    ERROR("{}: could not set volume: Audio stream {} not found",
          audio::subsystem_name, id);
    return std::unexpected(audio::error::stream_not_found);
  }

  ma_sound_group_set_volume(stream, volume);
  
  INFO("{}: volume for stream {} set to {}",
       audio::subsystem_name, id, volume);
  return {};
}

std::expected<void, audio::error>
audio::stream_stop(const audio::stream_id_t &id)
{
  audio::stream_t *stream = audio::get_stream(id);
  if (stream == nullptr)
  {
    ERROR("{}: could not pause stream: stream {} not found",
          audio::subsystem_name, id);
    return std::unexpected(audio::error::stream_not_found);
  }
  
  if (ma_sound_group_stop(stream) != MA_SUCCESS)
  {
    ERROR("{}: error stopping stream {}",
          audio::subsystem_name, id);
    return std::unexpected(audio::error::stream_stop);
  }
  
  INFO("{}: stream {} stopped", audio::subsystem_name, id);
  return {};
}

std::expected<void, audio::error>
audio::stream_start(const audio::stream_id_t &id)
{
  auto stream = audio::get_stream(id);
  if (stream == nullptr)
  {
    ERROR("{}: could not start stream: Audio stream {} not found",
          audio::subsystem_name, id);
    return std::unexpected(audio::error::stream_not_found);
  }

  if (ma_sound_group_start(stream) != MA_SUCCESS)
  {
    ERROR("{}: error starting stream {}", audio::subsystem_name, id);
    return std::unexpected(audio::error::stream_start);
  }

  INFO("{}: stream {} started", audio::subsystem_name, id);
  return {};
}

//
// Builder
//

audio::builder&
audio::builder::sound(const audio::sound_id_t &sound_id,
                      const std::string &path,
                      const audio::stream_id_t &stream_id)
{
  this->init_sounds.push_back(std::make_tuple(sound_id, path, stream_id));
  return *this;
}

audio::builder&
audio::builder::stream(const audio::stream_id_t &id,
                       float volume)
{
  this->init_streams.push_back(std::make_pair(id, volume));
  return *this;
}

subsystem &audio::builder::build()
{
  audio::init_sounds = this->init_sounds;
  audio::init_streams = this->init_streams;
  return audio::instance();
}
