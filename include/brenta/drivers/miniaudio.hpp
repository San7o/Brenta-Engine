// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/sound.hpp>

#include <miniaudio/miniaudio.h>

#include <tenno/array.hpp>
#include <deque>

#ifndef BRENTA_NUM_STREAMS
  #define BRENTA_NUM_STREAMS 64
#endif

namespace brenta
{

//
// SoundManager Driver using Miniaudio
// -----------------------------------
//
// The implementation for this driver is designed to be O(#sounds)
// when allocating new sounds, and O(1) for sound operations like
// play and stop.
//
// We keep a list of sound and stream slots, and whether they are
// being used or not. When we deallocate a sound or stream, we mark
// that as unused so that it may be used for future allocations. This
// reduces the number of heap memory allocations, but we iterate
// over the slots. The time complexity could be improved by keeping
// a free list, but I that also adds additional memory allocations
// and I don't think it is worth it.
//
// To invalidate indexed to deallocated / reallocated slots we keep
// track of which sound a stream is bound to. We identify a sound
// by its index in the sounds vector, and a generation number. When
// a new sound is loaded, the generation of its slot is increased,
// so we have an unique way to identify that sound.
//
class MiniaudioDriver : public SoundManager::Driver
{
public:

  using MiniaudioBuffer     = ma_audio_buffer;
  using MiniaudioStream     = ma_sound;

  struct SoundAssetSlot
  {
    MiniaudioBuffer  buffer;
    bool             in_use     = false;
    int              generation;

    SoundAssetSlot() = default;
    SoundAssetSlot(const SoundAssetSlot&) = delete;
    SoundAssetSlot &operator=(const SoundAssetSlot&) = delete;

    SoundAssetSlot(SoundAssetSlot&&)            = default;
    SoundAssetSlot &operator=(SoundAssetSlot&&) = default;
    
  };

  // An instance of a sound asset
  struct StreamSlot
  {
    MiniaudioStream    handle;
    MiniaudioBuffer    local_buffer;
    bool               in_use     = false;
    // We keep a sound_index + generation number to make sure that
    // this stream is referring to a particular valid sound
    int                generation;
    int                sound_index;

    StreamSlot() = default;
    StreamSlot(const StreamSlot&) = delete;
    StreamSlot &operator=(const StreamSlot&) = delete;

    StreamSlot(StreamSlot&&)            = default;
    StreamSlot &operator=(StreamSlot&&) = default;
    
  };
    
  MiniaudioDriver()  = default;
  MiniaudioDriver(const MiniaudioDriver&)            = delete;
  MiniaudioDriver &operator=(const MiniaudioDriver&) = delete;
  ~MiniaudioDriver() = default;

  std::expected<void, std::string> initialize() override;
  std::expected<void, std::string> terminate()  override;

  std::optional<Stream> request_stream(const SoundAsset& sound) override;
  void                  release_stream(Stream stream)           override;

  // Loading sounds is O(#sounds) 
  std::optional<SoundAsset>   load(const std::filesystem::path& path) override;
  void                        unload(const SoundAsset& sound)         override;

  // play / stop is O(1) on the number of sounds
  void play(Stream stream) override;
  void stop(Stream stream) override;
  void set_volume(Stream stream, float volume) override;
  
private:

  ma_engine engine;
  
  // SoundAssetId.index is an index in this vector
  // We use a deque becaues it does not reallocate obejcts, since
  // miniaudio decoders are not move safe!
  std::deque<SoundAssetSlot>                   sound_assets;
  // Stream is an index in this array
  // We use a fixed number of streams which are allocated and
  // deallocated in initialize() / terminate(). We keep track
  // of which slots are used and which ones are free.
  tenno::array<StreamSlot, BRENTA_NUM_STREAMS> stream_pool;

  std::optional<MiniaudioStream> create_stream();
  
};
  
} // namespace brenta
