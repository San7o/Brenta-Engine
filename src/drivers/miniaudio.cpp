// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/drivers/miniaudio.hpp>
#include <brenta/logger.hpp>

using namespace brenta;

//
// Static variables
//

std::expected<void, std::string> Miniaudio::initialize()
{
  ma_result result;
  result = ma_engine_init(NULL, &this->engine);
  if (result != MA_SUCCESS)
  {
    ERROR("Miniaudio: error initializing miniaudio driver: {}",
          ma_result_description(result));
    return std::unexpected("Initializing miniaudio driver");
  }
  
  INFO("Miniaudio: driver initialized");
  return {};
}

std::expected<void, std::string> Miniaudio::terminate()
{
  for (auto& sound : this->sounds)
    ma_sound_uninit(&sound.second);
  
  for (auto& stream : this->streams)
    ma_sound_group_uninit(&stream.second);

  ma_engine_uninit(&this->engine);

  INFO("Miniaudio: driver termianted");
  return {};
}

//
// Member functions
//

std::expected<void, Audio::Error>
Miniaudio::load(const Audio::SoundId &sound_id,
                const std::filesystem::path &path,
                const Audio::StreamId &stream_id)
{
  auto stream = this->get_stream(stream_id);
  if (!stream)
  {
    this->create_stream(stream_id);
    stream = this->get_stream(stream_id);
    if (!stream)
    {
      ERROR("Miniaudio: error stream {} not found", stream_id);
      return std::unexpected(Audio::Error::StreamNotFound);
    }
  }
    
  Sound sound = {};
  this->sounds.insert({sound_id, sound});
  if (ma_sound_init_from_file(&this->engine, path.string().c_str(),
                              0, (ma_sound_group*) stream.value(), NULL,
                              &this->sounds.at(sound_id)) != MA_SUCCESS)
  {
    ERROR("Miniaudio: error loading sound {} from path {}",
          sound_id, path.string());
    return std::unexpected(Audio::Error::InitFromFile);
  }
  
  INFO("Miniaudio: loaded sound {} from {} in stream {}",
       sound_id, path.string(), stream_id);
  return {};
}

std::expected<void, Audio::Error>
Miniaudio::play(const Audio::SoundId &id)
{
  Sound *sound = &this->sounds.at(id);
  if (!sound)
  {
    ERROR("Miniaudio: sound with id {} not found", id);
    return std::unexpected(Audio::Error::SoundNotFound);
  }
  ma_sound_start(sound);
  return {};
}

std::expected<void, Audio::Error>
Miniaudio::create_stream(const Audio::StreamId &id)
{
  auto stream_handle = this->get_stream(id);
  if (stream_handle) return {};

  Stream s = {};
  this->streams.insert({id, s});
  Stream* stream = &this->streams.at(id);
  if (ma_sound_group_init(&this->engine, 0, NULL, stream)
      != MA_SUCCESS)
  {
    ERROR("Miniaudio: error creating audio stream {}", id);
    return std::unexpected(Audio::Error::StreamInit);
  }
  
  INFO("Miniaudio: stream {} created", id);
  return {};
}

std::optional<Audio::StreamHandle>
Miniaudio::get_stream(const Audio::StreamId &id)
{
  if (this->streams.find(id) == this->streams.end())
    return {};
  return &this->streams.at(id);
}

std::expected<void, Audio::Error>
Miniaudio::stream_set_volume(const Audio::StreamId &id, float volume)
{
  auto stream = this->get_stream(id);
  if (!stream)
  {
    ERROR("Miniaudio: could not set volume: Audio stream {} not found",
          id);
    return std::unexpected(Audio::Error::StreamNotFound);
  }

  ma_sound_group_set_volume((ma_sound_group*) stream.value(), volume);
  
  INFO("Miniaudio: volume for stream {} set to {}", id, volume);
  return {};
}

std::expected<void, Audio::Error>
Miniaudio::stream_stop(const Audio::StreamId &id)
{
  auto stream = Audio::get_stream(id);
  if (!stream)
  {
    ERROR("Miniaudio: could not pause stream: stream {} not found", id);
    return std::unexpected(Audio::Error::StreamNotFound);
  }
  
  if (ma_sound_group_stop((ma_sound_group*) stream.value()) != MA_SUCCESS)
  {
    ERROR("Miniaudio: error stopping stream {}", id);
    return std::unexpected(Audio::Error::StreamStop);
  }
  
  INFO("Miniaudio: stream {} stopped", id);
  return {};
}

std::expected<void, Audio::Error>
Miniaudio::stream_start(const Audio::StreamId &id)
{
  auto stream = this->get_stream(id);
  if (!stream)
  {
    ERROR("Miniaudio: could not start stream: Audio stream {} not found",
          id);
    return std::unexpected(Audio::Error::StreamNotFound);
  }

  if (ma_sound_group_start((ma_sound_group*) stream.value()) != MA_SUCCESS)
  {
    ERROR("Miniaudio: error starting stream {}", id);
    return std::unexpected(Audio::Error::StreamStart);
  }

  INFO("Miniaudio: stream {} started", id);
  return {};
}
