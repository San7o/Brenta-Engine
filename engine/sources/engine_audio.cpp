
#include "engine_audio.h"
#include "engine_logger.h"
#include "SDL3/SDL_init.h"

#include <string>

using namespace ECS;

std::unordered_map<Types::AudioName, Types::AudioFile> Audio::audiofiles;
std::vector<SDL_AudioStream*> Audio::streams;

void Audio::Init()
{
    if (SDL_Init(SDL_INIT_AUDIO) ) {
        auto error = SDL_GetError();
        Logger::Log(Types::LogLevel::ERROR,
                        "SDL Audio failed to initialize: " + std::string(error));
        return;
    } 

    Audio::CreateStream();
    Logger::Log(Types::LogLevel::INFO,
                    "SDL Audio initialized");
}

void Audio::Destroy() 
{
    for (auto& stream : Audio::streams)
        SDL_DestroyAudioStream(stream);

    for (auto& audiofile : Audio::audiofiles) {
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
                            &audiofile.audio_buf, &audiofile.audio_len)) {
            auto error = SDL_GetError();
            Logger::Log(Types::LogLevel::ERROR,
                        "SDL Audio failed to load WAV file: " + std::string(error));
        return;
    }

    Audio::audiofiles.insert({name, audiofile});

    Logger::Log(Types::LogLevel::INFO,
                    "Loaded audio at " + path);
}

void Audio::PlayAudio(SDL_AudioStream* stream, Types::AudioName name) 
{
    auto audiofile = Audio::GetAudioFile(name);
    SDL_PutAudioStreamData(stream, audiofile.audio_buf, audiofile.audio_len);
}

void Audio::PlayAudio(Types::AudioName name) 
{
    auto audiofile = Audio::GetAudioFile(name);
    SDL_AudioStream* stream = Audio::streams.at(0);
    SDL_PutAudioStreamData(stream, audiofile.audio_buf, audiofile.audio_len);
}

void Audio::CreateStream() 
{
    SDL_AudioStream* stream = SDL_OpenAudioDeviceStream(
                    SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL, NULL, NULL);
    if (stream == NULL) {
        const char* error = SDL_GetError();
        Logger::Log(Types::LogLevel::ERROR,
                        "SDL Audio failed to create stream: " + std::string(error));
        return;
    }

    SDL_ResumeAudioStreamDevice(stream);
    Audio::streams.push_back(stream);
    Logger::Log(Types::LogLevel::INFO,
                    "SDL Audio stream created");
}

Types::AudioFile Audio::GetAudioFile(Types::AudioName name) 
{
    if (Audio::audiofiles.find(name) == Audio::audiofiles.end()) {
        Logger::Log(Types::LogLevel::ERROR,
                        "Audio file not found");
        return Types::AudioFile();
    }
    return Audio::audiofiles.at(name);
}
