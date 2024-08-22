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
 * The above copyright notice and this permission notice shall be included in all
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

#include "engine_audio.h"
#include "engine_logger.h"
#include "SDL3/SDL_init.h"

#include <string>

using namespace ECS;

std::unordered_map<Types::AudioName, Types::AudioFile> Audio::audiofiles;
std::unordered_map<Types::StreamName, SDL_AudioStream*> Audio::streams;

void Audio::Init()
{
    if (SDL_Init(SDL_INIT_AUDIO) )
    {
        auto error = SDL_GetError();
        Logger::Log(Types::LogLevel::ERROR,
                        "SDL Audio failed to initialize: " + std::string(error));
        return;
    } 

    Audio::CreateStream("default");
    Logger::Log(Types::LogLevel::INFO,
                    "SDL Audio initialized");
}

void Audio::Destroy() 
{
    for (auto& stream : Audio::streams)
        SDL_DestroyAudioStream(stream.second);

    for (auto& audiofile : Audio::audiofiles)
    {
        SDL_free(audiofile.second.audio_buf);
    }

    SDL_Quit();
    Logger::Log(Types::LogLevel::INFO,
                    "SDL Audio destroyed");
}

void Audio::LoadAudio(Types::AudioName name, std::string path)
{
    Types::AudioFile audiofile;
    audiofile.path = path;

    if (SDL_LoadWAV(path.c_str(), &audiofile.spec,
                            &audiofile.audio_buf, &audiofile.audio_len))
    {
            auto error = SDL_GetError();
            Logger::Log(Types::LogLevel::ERROR,
                        "SDL Audio failed to load WAV file: " + std::string(error));
        return;
    }

    Audio::audiofiles.insert({name, audiofile});
    Logger::Log(Types::LogLevel::INFO,
                    "Loaded audio at " + path);
}

void Audio::PlayAudio(Types::AudioName audio_name, Types::StreamName stream_name) 
{
    auto stream = Audio::GetStream(stream_name);
    if (stream == nullptr)
    {
        Logger::Log(Types::LogLevel::ERROR,
                        "Could not play audio: Audio stream not found");
        return;
    }

    ClearStream(stream_name);
    auto audiofile = Audio::GetAudioFile(audio_name);
    if (SDL_PutAudioStreamData(stream, audiofile.audio_buf, audiofile.audio_len))
        CheckError();
}

void Audio::CreateStream(Types::StreamName name) 
{
    SDL_AudioStream* stream = SDL_OpenAudioDeviceStream(
                    SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL, NULL, NULL);
    if (stream == NULL)
    {
        const char* error = SDL_GetError();
        Logger::Log(Types::LogLevel::ERROR,
                        "SDL Audio failed to create stream: " + std::string(error));
        return;
    }

    Audio::streams.insert({name, stream});
    ResumeStream(name);
    Logger::Log(Types::LogLevel::INFO,
                    "SDL Audio stream created");
}

Types::AudioFile Audio::GetAudioFile(Types::AudioName name) 
{
    if (Audio::audiofiles.find(name) == Audio::audiofiles.end())
    {
        Logger::Log(Types::LogLevel::ERROR,
                        "Audio file not found with name: " + name);
        return Types::AudioFile();
    }
    return Audio::audiofiles.at(name);
}

SDL_AudioStream* Audio::GetStream(Types::StreamName name) 
{
    if (Audio::streams.find(name) == Audio::streams.end())
    {
        Logger::Log(Types::LogLevel::ERROR,
                        "Audio stream not found with name: " + name);
        return nullptr;
    }
    return Audio::streams.at(name);
}

void Audio::SetVolume(Types::StreamName name, int volume) 
{
    auto stream = Audio::GetStream(name);
    if (stream == nullptr)
    {
        Logger::Log(Types::LogLevel::ERROR,
                        "Could not set volume: Audio stream not found");
        return;
    }

    if (SDL_SetAudioStreamGain(stream, volume)) CheckError();
    Logger::Log(Types::LogLevel::INFO,
                    "Volume set to " + std::to_string(volume));
}

void Audio::CheckError() 
{
    auto error = SDL_GetError();
    Logger::Log(Types::LogLevel::ERROR,
                    "SDL Audio error: " + std::string(error));
}

void Audio::ClearStream(Types::StreamName name) 
{
    auto stream = Audio::GetStream(name);
    if (stream == nullptr)
    {
        Logger::Log(Types::LogLevel::ERROR,
                        "Could not clear stream: Audio stream not found");
        return;
    }

    if (SDL_ClearAudioStream(stream)) CheckError();
    Logger::Log(Types::LogLevel::INFO,
                    "Stream cleared");
}

void Audio::PauseStream(Types::StreamName name) 
{
    auto stream = Audio::GetStream(name);
    if (stream == nullptr)
    {
        Logger::Log(Types::LogLevel::ERROR,
                        "Could not pause stream: Audio stream not found");
        return;
    }

    if(SDL_PauseAudioStreamDevice(stream)) CheckError();
    Logger::Log(Types::LogLevel::INFO,
                    "Stream paused");
}

void Audio::ResumeStream(Types::StreamName name) 
{
    auto stream = Audio::GetStream(name);
    if (stream == nullptr)
    {
        Logger::Log(Types::LogLevel::ERROR,
                        "Could not resume stream: Audio stream not found");
        return;
    }

    if (SDL_ResumeAudioStreamDevice(stream)) CheckError();
    Logger::Log(Types::LogLevel::INFO,
                    "Stream resumed");
}
