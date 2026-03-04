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

std::expected<void, std::string> MiniaudioDriver::initialize()
{
  ma_result result;
  result = ma_engine_init(NULL, &this->engine);
  if (result != MA_SUCCESS)
  {
    ERROR("Miniaudio: error initializing miniaudio driver: {}",
          ma_result_description(result));
    return std::unexpected("Initializing miniaudio driver");
  }

  for (int i = 0; i < BRENTA_NUM_STREAMS; ++i)
  {
    this->stream_pool[i].in_use      = false;
    this->stream_pool[i].generation  = -1;
    this->stream_pool[i].sound_index = -1;
  }
  
  INFO("Miniaudio: driver initialized");
  return {};
}

std::expected<void, std::string> MiniaudioDriver::terminate()
{
  ma_engine_uninit(&this->engine);

  // Clean up the stram pool
  for (int i = 0; i < BRENTA_NUM_STREAMS; ++i)
  {
    auto& slot = this->stream_pool[i];
    if (slot.in_use)
    {
      ma_sound_uninit(&slot.handle);
      ma_audio_buffer_uninit(&slot.local_buffer);
      slot.in_use = false;
    }
  }

  // Clean up the Asset Pool
  for (auto& asset : this->sound_assets)
  {
    if (asset.in_use)
    {
      ma_audio_buffer_uninit(&asset.buffer);
      asset.in_use = false;
    }
  }
    
  this->sound_assets.clear();

  INFO("Miniaudio: driver terminated safely");
  return {};
}

//
// Member functions
//

std::optional<MiniaudioDriver::MiniaudioStream> MiniaudioDriver::create_stream()
{
  MiniaudioStream stream_handle;
  ma_result res = ma_sound_init_ex(&this->engine, NULL, &stream_handle);
  if (res != MA_SUCCESS)
  {
    ERROR("Miniaudio: error creating audio stream");
    return {};
  }

  return stream_handle;
}

std::optional<Stream>
MiniaudioDriver::request_stream(const SoundAsset& sound)
{
  for (int i = 0; i < BRENTA_NUM_STREAMS; ++i)
  {
    if (!this->stream_pool[i].in_use)
    {
      auto& slot  = this->stream_pool[i];
      auto& asset = this->sound_assets[sound.get_id()];

      // Miniaudio boilerplate from here
      
      ma_format format;
      ma_uint32 channels;
      ma_uint32 sampleRate;
      ma_data_source_get_data_format(&asset.buffer, &format, &channels, &sampleRate, NULL, 0);

      ma_uint64 frameCount;
      ma_data_source_get_length_in_pcm_frames(&asset.buffer, &frameCount);
    
      void* pData = const_cast<void*>(asset.buffer.ref.pData);

      // 3. Initialize the streams's loal buffer
      ma_audio_buffer_config config =
        ma_audio_buffer_config_init(format, 
                                    channels, 
                                    frameCount, 
                                    pData, 
                                    NULL);
      config.sampleRate = sampleRate;

      ma_result res = ma_audio_buffer_init(&config, &slot.local_buffer);
      if (res != MA_SUCCESS)
      {
        ma_audio_buffer_uninit(&slot.local_buffer);
        ERROR("Miniaudio: Failed to create local buffer in sound {}",
              sound.get_id(), i);
        return {};
      }

      // Initialize the sound using the local_buffer
      res = ma_sound_init_from_data_source(&this->engine,
                                           &slot.local_buffer, 
                                           0, 
                                           NULL,
                                           &slot.handle);
      if (res != MA_SUCCESS)
      {
        ma_audio_buffer_uninit(&slot.local_buffer);
        ERROR("Miniaudio: Failed to bind sound {} to stream {}",
              sound.get_id(), i);
        return {};
      }

      DEBUG("Miniaudio: Binded sound {} to stream {}",
            sound.get_id(), i);

      //ma_sound_start(&slot.handle);
      this->stream_pool[i].in_use      = true;
      this->stream_pool[i].sound_index = sound.get_id();
      this->stream_pool[i].generation  = asset.generation;
      return i;
    }
  }
  return {};
}

void MiniaudioDriver::release_stream(Stream stream)
{
  auto& slot = this->stream_pool[stream];

  if (!slot.in_use)
    return;
  
  ma_sound_stop(&slot.handle);
  ma_sound_uninit(&slot.handle);
  ma_audio_buffer_uninit(&slot.local_buffer);

  slot.in_use = false;
  slot.sound_index = -1;
  slot.generation++; 

  DEBUG("Miniaudio: unbinded stream {}", stream);
}

std::optional<SoundAsset>
MiniaudioDriver::load(const std::filesystem::path& path)
{
  int slot_index = 0;
  bool found;
  
  for (unsigned int i = 0; i < this->sound_assets.size(); ++i)
  {
    if (!this->sound_assets[i].in_use)
    {
      found = true;
      slot_index = i;
      break;
    }
  }
  if (!found)
  {
    slot_index = sound_assets.size();
    this->sound_assets.emplace_back();
  }

  auto& sound_slot = this->sound_assets[slot_index];

  ma_decoder tmp_decoder;
  ma_result res =
    ma_decoder_init_file(path.string().c_str(),
                         NULL,
                         &tmp_decoder);
  if (res != MA_SUCCESS)
  {
    ERROR("Miniaudio: error loading sound decoder from path {}",
          path.string());
    return {};
  }

  ma_uint64 total_frames;
  ma_decoder_get_length_in_pcm_frames(&tmp_decoder, &total_frames);

  size_t bufferSize = total_frames * tmp_decoder.outputChannels
    * ma_get_bytes_per_sample(tmp_decoder.outputFormat);
  void* pRawData = malloc(bufferSize);

  ma_uint64 frames_read;
    ma_decoder_read_pcm_frames(&tmp_decoder, pRawData, total_frames, &frames_read);
  
  ma_audio_buffer_config config =
    ma_audio_buffer_config_init(tmp_decoder.outputFormat,
                                tmp_decoder.outputChannels,
                                total_frames,
                                pRawData,
                                NULL);
  res =
    ma_audio_buffer_init_copy(&config, &sound_slot.buffer);
  if (res != MA_SUCCESS)
  {
    ERROR("Miniaudio: error loading sound buffer from path {}",
          path.string());
    return {};
  }

  free(pRawData);
  ma_decoder_uninit(&tmp_decoder);
  
  sound_slot.in_use = true;
  sound_slot.generation++;
  
  INFO("Miniaudio: loaded sound {} from {}",
       slot_index, path.string());
  
  return slot_index;
}

void MiniaudioDriver::unload(const SoundAsset& sound)
{
  auto id = sound.get_id();
    
  if (id < 0 || !this->sound_assets[id].in_use)
    return;

  auto& asset_slot = this->sound_assets[id];

  // Disconnect any streams currently playing this asset
  for (int i = 0; i < BRENTA_NUM_STREAMS; ++i)
  {
    auto& stream = this->stream_pool[i];
        
    if (stream.in_use && stream.sound_index == id)
    {
      this->release_stream(i); 
      DEBUG("Miniaudio: Force-released stream {} because asset {} was unloaded", i, id);
    }
  }

  ma_audio_buffer_uninit(&asset_slot.buffer);

  asset_slot.in_use = false;
  asset_slot.generation++;
  return;
}

void MiniaudioDriver::play(Stream stream)
{
  auto& stream_slot = this->stream_pool[stream];
  if (!stream_slot.in_use)
  {
    ERROR("Miniaudio: play: cannot use stream {}", stream);
    return;
  }
  if (stream_slot.generation
      != this->sound_assets[stream_slot.sound_index].generation)
  {
    ERROR("Miniaudio: play: stream {} does not match driver's stream", stream);
    return;
  }
  
  ma_sound_start(&stream_slot.handle);

  INFO("Miniaudio: started playing on stream {}", stream);
  return;
}

void MiniaudioDriver::stop(Stream stream)
{
  auto& stream_slot = this->stream_pool[stream];
  if (!stream_slot.in_use)
  {
    ERROR("Miniaudio: stop: cannot use stream {}", stream);
    return;
  }
  if (stream_slot.generation
      != this->sound_assets[stream_slot.sound_index].generation)
  {
    ERROR("Miniaudio: stop: stream {} does not match driver's stream", stream);
    return;
  }
  
  ma_sound_stop(&stream_slot.handle);
  
  INFO("Miniaudio: stopped stream {}", stream);
  return;
}

void MiniaudioDriver::set_volume(Stream stream, float volume)
{
  auto& stream_slot = this->stream_pool[stream];
  if (!stream_slot.in_use)
  {
    ERROR("Miniaudio: stop: cannot use stream {}", stream);
    return;
  }
  if (stream_slot.generation
      != this->sound_assets[stream_slot.sound_index].generation)
  {
    ERROR("Miniaudio: stop: stream {} does not match driver's stream", stream);
    return;
  }
  
  ma_sound_set_volume(&stream_slot.handle, volume);
    
  INFO("Miniaudio: volume for stream {} set to {}", stream, volume);
  return;
}
