/*
 * MIT License
 *
 * Copyright (c) 2024 Giovanni Santini

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "engine_audio.hpp"

#include "SDL3/SDL_init.h"
#include "engine_logger.hpp"

#include <string>

using namespace brenta;

std::unordered_map<types::audio_name_t, types::audio_file_t> audio::audio_files;
std::unordered_map<types::stream_name_t, SDL_AudioStream *> audio::streams;

void audio::init()
{
    if (SDL_Init(SDL_INIT_AUDIO))
    {
        auto error = SDL_GetError();
        ERROR("SDL Audio failed to initialize: {}", error);
        return;
    }

    audio::create_stream("default");
    INFO("SDL Audio initialized");
}

void audio::destroy()
{
    for (auto &stream : audio::streams)
        SDL_DestroyAudioStream(stream.second);

    for (auto &audiofile : audio::audio_files)
    {
        SDL_free(audiofile.second.audio_buf);
    }

    SDL_Quit();
    INFO("SDL Audio destroyed");
}

void audio::load_audio(types::audio_name_t name, std::string path)
{
    types::audio_file_t audiofile;
    audiofile.path = path;

    if (SDL_LoadWAV(path.c_str(), &audiofile.spec, &audiofile.audio_buf,
                    &audiofile.audio_len))
    {
        auto error = SDL_GetError();
        ERROR("SDL Audio failed to load WAV file: {}", error);
        return;
    }

    audio::audio_files.insert({name, audiofile});
    INFO("Loaded audio at {}", path);
}

void audio::play_audio(types::audio_name_t audio_name,
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
    if (SDL_PutAudioStreamData(stream, audiofile.audio_buf,
                               audiofile.audio_len))
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

void audio::set_volume(types::stream_name_t name, int volume)
{
    auto stream = audio::get_stream(name);
    if (stream == nullptr)
    {
        ERROR("Could not set volume: Audio stream not found");
        return;
    }

    if (SDL_SetAudioStreamGain(stream, volume))
        check_error_audio();
    INFO("Volume set to {}", volume);
}

void audio::check_error_audio()
{
    auto error = SDL_GetError();
    ERROR("SDL Audio error: {}", error);
}

void audio::clear_stream(types::stream_name_t name)
{
    auto stream = audio::get_stream(name);
    if (stream == nullptr)
    {
        ERROR("Could not clear stream: Audio stream not found");
        return;
    }

    if (SDL_ClearAudioStream(stream))
        check_error_audio();
    INFO("Stream cleared");
}

void audio::pause_stream(types::stream_name_t name)
{
    auto stream = audio::get_stream(name);
    if (stream == nullptr)
    {
        ERROR("Could not pause stream: Audio stream not found");
        return;
    }

    if (SDL_PauseAudioStreamDevice(stream))
        check_error_audio();
    INFO("Stream paused");
}

void audio::resume_stream(types::stream_name_t name)
{
    auto stream = audio::get_stream(name);
    if (stream == nullptr)
    {
        ERROR("Could not resume stream: Audio stream not found");
        return;
    }

    if (SDL_ResumeAudioStreamDevice(stream))
        check_error_audio();
    INFO("Stream resumed");
}
