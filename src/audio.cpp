// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/audio.hpp>
#include <brenta/logger.hpp>

// Backend
#include <brenta/drivers/miniaudio.hpp>

#include <tuple>

using namespace brenta;

//
// Static variables
//

tenno::vector<std::tuple<Audio::SoundId, std::filesystem::path,
                         Audio::StreamId>> Audio::init_sounds;
tenno::vector<std::pair<Audio::StreamId, float>> Audio::init_streams;
tenno::shared_ptr<AudioDriver> Audio::backend = nullptr;
bool Audio::initialized = false;
const std::string Audio::subsystem_name = "audio";

//
// Subsystem interface
//

std::expected<void, Subsystem::Error> Audio::initialize()
{
  if (this->is_initialized()) return {};

  this->backend = tenno::make_shared<Miniaudio>();
  
  auto res = this->backend->initialize();
  if (!res) return res;

  std::expected<void, brenta::Audio::Error> result;
  for (auto& f : Audio::init_sounds)
  {
    result = this->backend->load(std::get<0>(f), std::get<1>(f), std::get<2>(f));
    if (!result) return std::unexpected("Error loading audio " + std::get<0>(f));
  }

  for (auto& s : Audio::init_streams)
  {
    result = this->backend->create_stream(s.first);
    if (!result) return std::unexpected("Error creating stream " + s.first);
    result = this->backend->stream_set_volume(s.first, s.second);
    if (!result) return std::unexpected("Error setting volume for stream " + s.first);
  }

  if (!this->backend->get_stream("default"))
  {
    result = this->backend->create_stream("default");
    if (!result) return std::unexpected("Error creating stream \"default\"");
  }

  Audio::initialized = true;
  INFO("{}: initialized", Audio::subsystem_name);
  return {};
}

std::expected<void, Subsystem::Error> Audio::terminate()
{
  if (!this->is_initialized()) return {};

  this->backend->terminate();

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
            const std::filesystem::path &path,
            const Audio::StreamId &stream_id)
{
  if (!Audio::backend)
    return std::unexpected(Audio::Error::Uninitialized);
  return Audio::backend->load(sound_id, path, stream_id);
}

std::expected<void, Audio::Error>
Audio::play(const Audio::SoundId &id)
{
  if (!Audio::backend)
    return std::unexpected(Audio::Error::Uninitialized);
  return Audio::backend->play(id);
}

std::expected<void, Audio::Error>
Audio::create_stream(const Audio::StreamId &id)
{
  if (!Audio::backend)
    return std::unexpected(Audio::Error::Uninitialized);
  return Audio::backend->create_stream(id);
}

std::optional<Audio::StreamHandle>
Audio::get_stream(const Audio::StreamId &id)
{
  if (!Audio::backend)
    return nullptr;
  return Audio::backend->get_stream(id);
}

std::expected<void, Audio::Error>
Audio::stream_set_volume(const Audio::StreamId &id, float volume)
{
  if (!Audio::backend)
    return std::unexpected(Audio::Error::Uninitialized);
  return Audio::backend->stream_set_volume(id, volume);
}

std::expected<void, Audio::Error>
Audio::stream_stop(const Audio::StreamId &id)
{
  if (!Audio::backend)
    return std::unexpected(Audio::Error::Uninitialized);
  return Audio::backend->stream_stop(id);
}

std::expected<void, Audio::Error>
Audio::stream_start(const Audio::StreamId &id)
{
  if (!Audio::backend)
    return std::unexpected(Audio::Error::Uninitialized);
  return Audio::backend->stream_start(id);
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
  Audio::init_sounds  = this->init_sounds;
  Audio::init_streams = this->init_streams;
  return Audio::instance();
}
