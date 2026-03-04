// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/sound.hpp>
#include <brenta/drivers/miniaudio.hpp>
#include <brenta/logger.hpp>

using namespace brenta;

//
// Sound Asset
//

SoundAsset::~SoundAsset()
{
  SoundManager::unload(*this);
}

SoundAsset::SoundAssetId SoundAsset::get_id() const
{
  return this->id;
}

SoundAsset::Builder
&SoundAsset::Builder::path(const std::filesystem::path& path)
{
  this->_path = path;
  return *this;
}

std::optional<SoundAsset> SoundAsset::Builder::build()
{
  return SoundManager::load(this->_path);
}

//
// Sound
//

Sound::Sound(const SoundAsset& asset)
{
  auto maybe_stream = SoundManager::request_stream(asset);
  if (!stream)
    return;
  
  this->stream = maybe_stream.value();
}

Sound::~Sound()
{
  SoundManager::release_stream(this->stream);
}

void Sound::play()
{
  SoundManager::play(*this);
}

void Sound::stop()
{
  SoundManager::stop(*this);
}

void Sound::set_volume(float v)
{
  SoundManager::set_volume(*this, v);
}
  
//
// SoundManager
//

// Static variables

const std::string                         SoundManager::subsystem_name = "SoundManager";
tenno::shared_ptr<SoundManager::Driver>   SoundManager::driver;
bool                                      SoundManager::initialized    = false;


// Subsystem interface

std::expected<void, Subsystem::Error> SoundManager::initialize()
{
  if (SoundManager::initialized)
    return {};
  
  this->driver = tenno::make_shared<MiniaudioDriver>();
  auto ret = SoundManager::driver->initialize();
  if (!ret)
    return ret;

  SoundManager::initialized = true;

  DEBUG("{}: initialized", SoundManager::subsystem_name);
  
  return ret;
}

std::expected<void, Subsystem::Error> SoundManager::terminate()
{
  if (!SoundManager::initialized | !SoundManager::driver)
    return {};

  auto ret = SoundManager::driver->terminate();
  if (!ret)
    return ret;

  SoundManager::initialized = false;
  
  DEBUG("{}: terminated", SoundManager::subsystem_name);
  
  return ret;
}

std::string SoundManager::name()
{
  return SoundManager::subsystem_name;
}

bool SoundManager::is_initialized()
{
  return SoundManager::initialized;
}

// Member functions
  
SoundManager &SoundManager::instance()
{
  static SoundManager _instance;
  return _instance;
}

std::optional<Stream> SoundManager::request_stream(const SoundAsset& asset)
{
  if (!SoundManager::initialized || asset.id < 0)
    return {};
  return SoundManager::driver->request_stream(asset);
}

void SoundManager::release_stream(Stream stream)
{
  if (!SoundManager::initialized || stream < 0)
    return;
  SoundManager::driver->release_stream(stream);
  return;
}

std::optional<SoundAsset>
SoundManager::load(const std::filesystem::path& path)
{
  if (!SoundManager::initialized || !SoundManager::driver)
    return {};
  return SoundManager::driver->load(path);
}

void SoundManager::unload(const SoundAsset& sound)
{
  if (!SoundManager::initialized
      || !SoundManager::driver
      || sound.id < 0)
    return;
  return SoundManager::driver->unload(sound);
}

void SoundManager::play(const Sound& sound)
{
  if (!SoundManager::initialized || !SoundManager::driver
      || sound.stream < 0)
    return;
  return SoundManager::driver->play(sound.stream);
}

void SoundManager::stop(const Sound& sound)
{
  if (!SoundManager::initialized || !SoundManager::driver
      || sound.stream < 0)
    return;
  return SoundManager::driver->stop(sound.stream);
}

void SoundManager::set_volume(const Sound& sound, float volume)
{
  if (!SoundManager::initialized || !SoundManager::driver
      || sound.stream < 0)
    return;
  return SoundManager::driver->set_volume(sound.stream, volume);
}

//
// Builder
//

Subsystem &SoundManager::Builder::build()
{
  return SoundManager::instance();
}
