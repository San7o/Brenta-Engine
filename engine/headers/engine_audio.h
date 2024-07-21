#pragma once

#ifndef ENGINE_AUDIO_H
#define ENGINE_AUDIO_H

#include <SDL3/SDL_audio.h>
#include <unordered_map>
#include <string>
#include <vector>

namespace ECS {

namespace Types {

typedef std::string StreamName;

typedef std::string AudioName;

struct AudioFile
{
    std::string path;        // Path to audio file
    Uint8* audio_buf;       // Pointer to audio buffer
    Uint32 audio_len;       // Length of audio buffer
    SDL_AudioSpec spec;     // Information about the audio format
};

/*
 * typedef struct SDL_AudioSpec
 * {
 *   SDL_AudioFormat format; // Audio data format
 *   int channels;           // Number of channels: 1 mono, 2 stereo, etc
 *   int freq;               // sample rate: sample frames per second
 * } SDL_AudioSpec;
*
*/

} // namespace Types

class Audio
{
public:
    static std::unordered_map<Types::AudioName, Types::AudioFile> audiofiles;
    static std::unordered_map<Types::StreamName, SDL_AudioStream*> streams;
    
    Audio() = delete;
    static void Init();
    static void Destroy();

    static Types::AudioFile GetAudioFile(Types::AudioName name);
    static SDL_AudioStream* GetStream(Types::StreamName name);

    static void LoadAudio(Types::AudioName name, std::string path);
    static void CreateStream(Types::StreamName);
    static void PlayAudio(Types::AudioName, Types::StreamName = "default");
    static void SetVolume(Types::StreamName name, int volume);
    static void PauseStream(Types::StreamName name);
    static void ResumeStream(Types::StreamName name);
    static void ClearStream(Types::StreamName name);

private:
    static void CheckError();
};

} // namespace ECS

#endif
