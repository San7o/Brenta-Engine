// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <SDL3/SDL_init.h>
#include <brenta/audio.hpp>
#include <brenta/logger.hpp>
#include <string>

using namespace brenta;

std::vector<std::pair<types::audio_name_t, std::string>> audio::init_files;
std::vector<std::pair<types::stream_name_t, float>> audio::init_streams;
std::unordered_map<types::audio_name_t, types::audio_file_t> audio::audio_files;
std::unordered_map<types::stream_name_t, SDL_AudioStream *> audio::streams;

std::expected<void, std::string> audio::initialize()
{
  if (!SDL_Init(SDL_INIT_AUDIO))
  {
    auto error = SDL_GetError();
    ERROR("SDL Audio failed to initialize: {}", error);
    return std::unexpected(this->subsystem_name);
  }

  for (auto& f : init_files)
  {
    load(f.first, f.second);
  }

  for (auto& s : init_streams)
  {
    create_stream(s.first);
    set_volume(s.first, s.second);
  }

  create_stream("default");
  INFO("audio initialized");
  return {};
}

std::expected<void, std::string> audio::terminate()
{
  for (auto &stream : audio::streams)
    SDL_DestroyAudioStream(stream.second);

  for (auto &audiofile : audio::audio_files)
  {
    SDL_free(audiofile.second.audio_buf);
  }

  SDL_Quit();
  INFO("audio termianted");
  return {};
}

audio &audio::instance()
{
  static audio _audio;
  return _audio;
}

void audio::load(types::audio_name_t name, std::string path)
{
  types::audio_file_t audiofile;
  audiofile.path = path;

  if (!SDL_LoadWAV(path.c_str(), &audiofile.spec, &audiofile.audio_buf,
                   &audiofile.audio_len))
  {
    auto error = SDL_GetError();
    ERROR("SDL Audio failed to load WAV file: {}", error);
    return;
  }

  audio::audio_files.insert({name, audiofile});
  INFO("Loaded audio at {}", path);
}

void audio::play(types::audio_name_t audio_name,
                 types::stream_name_t stream_name)
{
  auto stream = audio::get_stream(stream_name);
  if (stream == nullptr)
  {
    ERROR("Could not play audio: Audio stream not found");
    return;
  }

  clear_stream(stream_name);
  auto audiofile = audio::get_audio_file(audio_name);
  if (!SDL_PutAudioStreamData(stream, audiofile.audio_buf, audiofile.audio_len))
    check_error_audio();
}

void audio::create_stream(types::stream_name_t name)
{
  SDL_AudioStream *stream = SDL_OpenAudioDeviceStream(
    SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL, NULL, NULL);
  if (stream == NULL)
  {
    const char *error = SDL_GetError();
    ERROR("SDL Audio failed to create stream: {}", error);
    return;
  }

  audio::streams.insert({name, stream});
  resume_stream(name);
  INFO("SDL Audio stream created");
}

types::audio_file_t audio::get_audio_file(types::audio_name_t name)
{
  if (audio::audio_files.find(name) == audio::audio_files.end())
  {
    ERROR("Audio file not found with name: {}", name);
    return types::audio_file_t();
  }
  return audio::audio_files.at(name);
}

SDL_AudioStream *audio::get_stream(types::stream_name_t name)
{
  if (audio::streams.find(name) == audio::streams.end())
  {
    ERROR("Audio stream not found with name: {}", name);
    return nullptr;
  }
  return audio::streams.at(name);
}

void audio::set_volume(types::stream_name_t name, float gain)
{
  auto stream = audio::get_stream(name);
  if (stream == nullptr)
  {
    ERROR("Could not set volume: Audio stream not found");
    return;
  }

  if (!SDL_SetAudioStreamGain(stream, gain))
    check_error_audio();
  INFO("Volume set to {}", gain);
}

void audio::check_error_audio()
{
  auto error = SDL_GetError();
  ERROR("SDL Audio error: {}", error);
}

void audio::clear(types::stream_name_t name)
{
  auto stream = audio::get_stream(name);
  if (stream == nullptr)
  {
    ERROR("Could not clear stream: Audio stream not found");
    return;
  }

  if (!SDL_ClearAudioStream(stream))
    check_error_audio();
  INFO("Stream cleared");
}

void audio::pause(types::stream_name_t name)
{
  auto stream = audio::get_stream(name);
  if (stream == nullptr)
  {
    ERROR("Could not pause stream: Audio stream not found");
    return;
  }

  if (!SDL_PauseAudioStreamDevice(stream))
    check_error_audio();
  INFO("Stream paused");
}

void audio::resume(types::stream_name_t name)
{
  auto stream = audio::get_stream(name);
  if (stream == nullptr)
  {
    ERROR("Could not resume stream: Audio stream not found");
    return;
  }

  if (!SDL_ResumeAudioStreamDevice(stream))
    check_error_audio();
  INFO("Stream resumed");
}

//
// Builder
//

audio::builder &audio::builder::load(types::audio_name_t id, std::string path)
{
  this->init_files.push_back(std::make_pair(id, path));
  return *this;
}

audio::builder &audio::builder::stream(types::stream_name_t name, float gain)
{
  this->init_streams.push_back(std::make_pair(name, gain));
  return *this;
}

subsystem &audio::builder::build()
{
  audio::init_files = this->init_files;
  audio::init_streams = this->init_streams;
  return audio::instance();
}
