// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/subsystem.hpp>

#include <tenno/memory.hpp>

#include <filesystem>

namespace brenta
{
  
// An identifier for a stream
typedef int Stream;

class SoundManager;
  
//
// SoundAsset
// ----------
//
// The sound assets stores all the information of an audio file. You
// cannot play a `SoundAsset` because it is just data, you can play a
// `Sound` instead.
// 
// A sound asset needs to be created from SoundManager::load(...).  It
// uses RAII to automatically deallocate its resources.
//
class SoundAsset
{
public:

  // Nagetive values are considered invalid
  using SoundAssetId = int;
  
  class Builder;
  friend class SoundManager;

  SoundAsset() = default;
  SoundAsset(SoundAssetId id)
    : id(id) {}
  SoundAsset(SoundAsset&& other)
  {
    this->id  = other.id;
    other.id  = -1;
  }
  ~SoundAsset();

  SoundAssetId get_id() const;

private:

  SoundAssetId id = -1;
  
};

class SoundAsset::Builder
{
public:

  Builder& path(const std::filesystem::path& path);

  std::optional<SoundAsset> build();

private:

  std::filesystem::path _path;
  
};

//
// Sound
// -----
//
// This class represents an instance of a sound asset that can be
// played.
//
// It uses RAII to automatically allocate / deallocate its stream in
// from the `SoundManager`.
//  
// To play the sound you need to have a sound asset first, then you
// can construct the sound simply by:
//
//     Sound sound = Sound(my_sound_sset);
//     sound.play();
//
class Sound
{
public:

  // Nagetive values are considered invalid
  friend class SoundManager;
  
  Sound() = default;
  // A sound is constructed from a sound asset, which you can get from
  // the AssetManager or SoundManager
  Sound(const SoundAsset& asset);
  Sound(Sound&& other)
  {
    this->stream = other.stream;
    other.stream = -1;
  }
  Sound(const Sound&)            = delete;
  Sound &operator=(const Sound&) = delete;
  ~Sound();

  void play();
  void stop();
  void set_volume(float v);
  
private:

  Stream  stream  = -1;
  
};

//
// SoundManager
// ------------
//
// Primary interface to manage all sound resources.
//
// The sound manager itself is a light wrapper around a sound
// `Driver`, which actually implements the entire logic.
//
class SoundManager : public Subsystem
{
public:

  class Driver;
  class Builder;

  //
  // Subsystem interface
  //
  
  static const std::string subsystem_name;
  std::string                           name()           override;
  bool                                  is_initialized() override;
  std::expected<void, Subsystem::Error> initialize()     override;
  std::expected<void, Subsystem::Error> terminate()      override;
  
  //
  // Member functions
  //
  
  static SoundManager &instance();

  static std::optional<SoundAsset> load(const std::filesystem::path& path);
  static void                      unload(const SoundAsset& sound);

  static std::optional<Stream> request_stream(const SoundAsset& id);
  static void                  release_stream(Stream stream);
  
  static void play(const Sound& sound);
  static void stop(const Sound& sound);
  static void set_volume(const Sound& sound, float volume);

private:

  static bool                                    initialized;
  static tenno::shared_ptr<Driver>               driver;

  SoundManager() = default;

};
  
class SoundManager::Driver
{
public:

  virtual ~Driver() = default;
  
  virtual std::expected<void, std::string> initialize() = 0;
  virtual std::expected<void, std::string> terminate()  = 0;

  virtual std::optional<Stream> request_stream(const SoundAsset& sound)  = 0;
  virtual void                  release_stream(Stream stream)       = 0;

  virtual std::optional<SoundAsset>   load(const std::filesystem::path& path) = 0;
  virtual void                        unload(const SoundAsset& sound)         = 0;

  virtual void play(Stream stream) = 0;
  virtual void stop(Stream stream) = 0;
  virtual void set_volume(Stream stream, float volume) = 0;
  
};

class SoundManager::Builder : public Subsystem::Builder
{
public:
  
  Builder() = default;
  ~Builder() = default;

  brenta::Subsystem &build() override;
  
};
  
} // namespace brenta
