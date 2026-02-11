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

std::vector<std::tuple<Audio::SoundId, std::filesystem::path,
                       Audio::StreamId>> Audio::init_sounds;
std::vector<std::pair<Audio::StreamId, float>> Audio::init_streams;
std::unordered_map<Audio::SoundId, Audio::Sound> Audio::sounds;
std::unordered_map<Audio::StreamId, Audio::Stream> Audio::streams;
ma_engine Audio::engine;
bool Audio::initialized = false;
const std::string Audio::subsystem_name = "audio";

//
// Subsystem interface
//

std::expected<void, Subsystem::Error> Audio::initialize()
{
  if (this->is_initialized()) return {};
  ma_result result;
  result = ma_engine_init(NULL, &Audio::engine);
  if (result != MA_SUCCESS)
  {
    ERROR("{}: error initializing engine: {}",
          Audio::subsystem_name, ma_result_description(result));
    return std::unexpected("Initializing audio backend");
  }

  for (auto& f : Audio::init_sounds)
  {
    if (!Audio::load(std::get<0>(f), std::get<1>(f), std::get<2>(f)).has_value())
      return std::unexpected("Loading audio " + get<0>(f));
  }

  for (auto& s : Audio::init_streams)
  {
    if (!Audio::create_stream(s.first).has_value())
      return std::unexpected("Creating stream " + s.first);
    if (!Audio::stream_set_volume(s.first, s.second).has_value())
      return std::unexpected("Setting volume for stream " + s.first);
  }

  if (!Audio::get_stream("default"))
  {
    if (!Audio::create_stream("default").has_value())
      return std::unexpected("Creating stream default");
  }

  Audio::initialized = true;
  INFO("{}: initialized", Audio::subsystem_name);
  return {};
}

std::expected<void, Subsystem::Error> Audio::terminate()
{
  if (!this->is_initialized()) return {};
  
  for (auto sound : Audio::sounds)
    ma_sound_uninit(&sound.second);
  
  for (auto stream : Audio::streams)
    ma_sound_group_uninit(&stream.second);

  ma_engine_uninit(&Audio::engine);

  Audio::initialized = false;
  INFO("{}: termianted", Audio::subsystem_name);
  return {};
}

std::string Audio::name()
{
  return Audio::subsystem_name;
}

bool Audio::is_initialized()
{
  return Audio::initialized;
}

//
// Member functions
//

Audio &Audio::instance()
{
  static Audio _audio;
  return _audio;
}

std::expected<void, Audio::Error>
Audio::load(const Audio::SoundId &sound_id,
            const std::string &path,
            const Audio::StreamId &stream_id)
{
  Audio::Stream *stream = Audio::get_stream(stream_id);
  if (!stream)
  {
    Audio::create_stream(stream_id);
    stream = Audio::get_stream(stream_id);
    if (!stream)
    {
      ERROR("{}: error stream {} not found",
            Audio::subsystem_name, stream_id);
      return std::unexpected(Audio::Error::StreamNotFound);
    }
  }

  Audio::Sound sound = {};
  Audio::sounds.insert({sound_id, sound});
  if (ma_sound_init_from_file(&Audio::engine, path.c_str(), 0, stream, NULL,
                              &Audio::sounds.at(sound_id)) != MA_SUCCESS)
  {
    ERROR("{}: error loading sound {} from path {}",
          Audio::subsystem_name, sound_id, path);
    return std::unexpected(Audio::Error::InitFromFile);
  }
  
  INFO("{}: loaded sound {} from {} in stream {}",
       Audio::subsystem_name, sound_id, path, stream_id);
  return {};
}

std::expected<void, Audio::Error>
Audio::play(const Audio::SoundId &id)
{
  Audio::Sound *sound = &Audio::sounds.at(id);
  if (!sound)
  {
    ERROR("{}: sound with id {} not found",
          Audio::subsystem_name, id);
    return std::unexpected(Audio::Error::SoundNotFound);
  }
  ma_sound_start(sound);
  return {};
}

std::expected<void, Audio::Error>
Audio::create_stream(const Audio::StreamId &id)
{
  Audio::Stream *stream = Audio::get_stream(id);
  if (stream) return {};

  Audio::Stream s = {};
  Audio::streams.insert({id, s});
  stream = &Audio::streams.at(id);
  if (ma_sound_group_init(&Audio::engine, 0, NULL, stream)
      != MA_SUCCESS)
  {
    ERROR("{}: error creating audio stream {}",
          Audio::subsystem_name, id);
    return std::unexpected(Audio::Error::StreamInit);
  }
  
  INFO("{}: stream {} created", Audio::subsystem_name, id);
  return {};
}

Audio::Stream *Audio::get_stream(const Audio::StreamId &id)
{
  if (Audio::streams.find(id) == Audio::streams.end())
  {
    return nullptr;
  }
  return &Audio::streams.at(id);
}

std::expected<void, Audio::Error>
Audio::stream_set_volume(const Audio::StreamId &id, float volume)
{
  Audio::Stream *stream = Audio::get_stream(id);
  if (!stream)
  {
    ERROR("{}: could not set volume: Audio stream {} not found",
          Audio::subsystem_name, id);
    return std::unexpected(Audio::Error::StreamNotFound);
  }

  ma_sound_group_set_volume(stream, volume);
  
  INFO("{}: volume for stream {} set to {}",
       Audio::subsystem_name, id, volume);
  return {};
}

std::expected<void, Audio::Error>
Audio::stream_stop(const Audio::StreamId &id)
{
  Audio::Stream *stream = Audio::get_stream(id);
  if (stream == nullptr)
  {
    ERROR("{}: could not pause stream: stream {} not found",
          Audio::subsystem_name, id);
    return std::unexpected(Audio::Error::StreamNotFound);
  }
  
  if (ma_sound_group_stop(stream) != MA_SUCCESS)
  {
    ERROR("{}: error stopping stream {}",
          Audio::subsystem_name, id);
    return std::unexpected(Audio::Error::StreamStop);
  }
  
  INFO("{}: stream {} stopped", Audio::subsystem_name, id);
  return {};
}

std::expected<void, Audio::Error>
Audio::stream_start(const Audio::StreamId &id)
{
  auto stream = Audio::get_stream(id);
  if (stream == nullptr)
  {
    ERROR("{}: could not start stream: Audio stream {} not found",
          Audio::subsystem_name, id);
    return std::unexpected(Audio::Error::StreamNotFound);
  }

  if (ma_sound_group_start(stream) != MA_SUCCESS)
  {
    ERROR("{}: error starting stream {}", Audio::subsystem_name, id);
    return std::unexpected(Audio::Error::StreamStart);
  }

  INFO("{}: stream {} started", Audio::subsystem_name, id);
  return {};
}

//
// Builder
//

Audio::Builder&
Audio::Builder::sound(const Audio::SoundId &sound_id,
                      const std::filesystem::path &path,
                      const Audio::StreamId &stream_id)
{
  this->init_sounds.push_back(std::make_tuple(sound_id, path, stream_id));
  return *this;
}

Audio::Builder&
Audio::Builder::stream(const Audio::StreamId &id,
                       float volume)
{
  this->init_streams.push_back(std::make_pair(id, volume));
  return *this;
}

Subsystem &Audio::Builder::build()
{
  Audio::init_sounds = this->init_sounds;
  Audio::init_streams = this->init_streams;
  return Audio::instance();
}
