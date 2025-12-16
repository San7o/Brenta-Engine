// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/audio.hpp>
#include <brenta/logger.hpp>
#include <string>

using namespace brenta;

std::vector<std::tuple<types::sound_id_t, std::string,
                       types::stream_id_t>> audio::init_sounds;
std::vector<std::pair<types::stream_id_t, float>> audio::init_streams;
std::unordered_map<types::sound_id_t, types::sound_t> audio::sounds;
std::unordered_map<types::stream_id_t, types::stream_t> audio::streams;
ma_engine audio::engine;

std::expected<void, std::string> audio::initialize()
{
  ma_result result;
  result = ma_engine_init(NULL, &audio::engine);
  if (result != MA_SUCCESS)
  {
    return std::unexpected(this->subsystem_name);
  }

  for (auto& f : audio::init_sounds)
  {
    audio::load(std::get<0>(f), std::get<1>(f), std::get<2>(f));
  }

  for (auto& s : audio::init_streams)
  {
    audio::create_stream(s.first);
    audio::stream_set_volume(s.first, s.second);
  }

  if (!audio::get_stream("default"))
    audio::create_stream("default");
  
  INFO("audio initialized");
  return {};
}

std::expected<void, std::string> audio::terminate()
{
  
  for (auto sound : audio::sounds)
    ma_sound_uninit(&sound.second);
  
  for (auto stream : audio::streams)
    ma_sound_group_uninit(&stream.second);

  ma_engine_uninit(&audio::engine);

  INFO("audio termianted");
  return {};
}

audio &audio::instance()
{
  static audio _audio;
  return _audio;
}

void audio::load(types::sound_id_t sound_id, std::string path,
                 types::stream_id_t stream_id)
{
  types::stream_t *stream = audio::get_stream(stream_id);
  if (!stream)
  {
    audio::create_stream(stream_id);
    stream = audio::get_stream(stream_id);
    if (!stream)
    {
      ERROR("Error creating stream {} for sound {}", stream_id, sound_id);
      return;
    }
  }

  types::sound_t sound = {};
  audio::sounds.insert({sound_id, sound});
  if (ma_sound_init_from_file(&audio::engine, path.c_str(), 0, stream, NULL,
                              &audio::sounds.at(sound_id)) != MA_SUCCESS)
  {
    ERROR("Error loading sound {} from path {}", sound_id, path);
    return;
  }
  
  INFO("Loaded sound {} from {} in stream {}", sound_id, path, stream_id);
  return;
}

void audio::play(types::sound_id_t id)
{
  types::sound_t *sound = &audio::sounds.at(id);
  if (!sound)
  {
    ERROR("Sound with id {} not found", id);
    return;
  }
  ma_sound_start(sound);
  return;
}

void audio::create_stream(types::stream_id_t id)
{
  types::stream_t *stream = audio::get_stream(id);
  if (stream) return;

  types::stream_t s = {};
  audio::streams.insert({id, s});
  stream = &audio::streams.at(id);
  if (ma_sound_group_init(&audio::engine, 0, NULL, stream)
      != MA_SUCCESS)
  {
    ERROR("Error creating audio stream {}", id);
    return;
  }
  
  INFO("Audio stream {} created", id);
}


types::stream_t *audio::get_stream(types::stream_id_t id)
{
  if (audio::streams.find(id) == audio::streams.end())
  {
    return nullptr;
  }
  return &audio::streams.at(id);
}

void audio::stream_set_volume(types::stream_id_t id, float volume)
{
  types::stream_t *stream = audio::get_stream(id);
  if (!stream)
  {
    ERROR("Could not set volume: Audio stream {} not found", id);
    return;
  }

  ma_sound_group_set_volume(stream, volume);
  
  INFO("Volume for {} set to {}", id, volume);
}

void audio::stream_stop(types::stream_id_t id)
{
  types::stream_t *stream = audio::get_stream(id);
  if (stream == nullptr)
  {
    ERROR("Could not pause stream: Audio stream {} not found", id);
    return;
  }
  
  if (ma_sound_group_stop(stream) != MA_SUCCESS)
  {
    ERROR("Error stopping stream {}", id);
    return;
  }
  
  INFO("Stream {} stopped", id);
}

void audio::stream_start(types::stream_id_t id)
{
  auto stream = audio::get_stream(id);
  if (stream == nullptr)
  {
    ERROR("Could not start stream: Audio stream {} not found", id);
    return;
  }

  if (ma_sound_group_stop(stream) != MA_SUCCESS)
  {
    ERROR("Error starting stream {}", id);
    return;
  }

  INFO("Stream resumed");
}

//
// Builder
//

audio::builder &audio::builder::sound(types::sound_id_t sound_id, std::string path,
                                      types::stream_id_t stream_id)
{
  this->init_sounds.push_back(std::make_tuple(sound_id, path, stream_id));
  return *this;
}

audio::builder &audio::builder::stream(types::stream_id_t id, float volume)
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
