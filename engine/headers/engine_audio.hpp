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

#pragma once

#include <SDL3/SDL_audio.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace brenta
{

namespace types
{

typedef std::string stream_name_t;

typedef std::string audio_name_t;

/**
 * @brief Struct containing information about an audio file
 *
 * This struct contains information about an audio file loaded by the engine.
 * It contains the path to the audio file, the audio buffer, the length of the
 * audio buffer and the audio format. It is autocamatically created by the
 * engine when an audio file is loaded with the LoadAudio function.
 */
struct audio_file_t
{
    /**
     * @brief The path to the audio file
     */
    std::string path;
    /**
     * @brief Pointer to audio buffer
     */
    Uint8 *audio_buf;
    /**
     * @brief Length of audio buffer
     */
    Uint32 audio_len;
    /**
     * @brief Information about the audio format
     */
    SDL_AudioSpec spec;
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

/**
 * @brief Audio subsystem
 *
 * This class contains the audio subsystem of the engine. It is used to load
 * audio files, create audio streams, play audio files on streams, set the
 * volume of streams, pause, resume and stop streams. The audio system
 * needs to be initialized and destroyed by the engine. Audio files and
 * streamd are stored in maps and are identified by this name.
 */
class audio
{
  public:
    /**
     * @brief Map of audio files
     *
     * This map contains all the audio files loaded by the engine.
     * The key is the name of the audio file provided by the user,
     * the value is the struct AudioFile containing the path to the
     * audio file, the audio buffer, the length of the audio buffer
     * and the audio format.
     */
    static std::unordered_map<types::audio_name_t, types::audio_file_t> audio_files;
    /**
     * @brief Map of audio streams
     *
     * This map contains all the audio streams created by the engine.
     * The key is the name of the stream provided by the user, the value
     * is the SDL_AudioStream pointer. A stream can only play one audio
     * file at a time, so multiple streams need to be created to play
     * multiple audio files at the same time. The engine creates a default
     * stream called "default", the handling of the streams is left to the
     * programmer.
     */
    static std::unordered_map<types::stream_name_t, SDL_AudioStream *> streams;

    audio() = delete;
    /**
     * @brief Initialize the audio system
     *
     * This function initializes the audio system of the engine.
     * It is called automatically by the engine when the game is
     * started. It creates a default stream called "default".
     */
    static void init();
    /**
     * @brief Destroy the audio system
     *
     * This function frees all audio streams and audio files,
     * and closes the audio system.
     */
    static void destroy();

    /**
     * @brief Get an audio file
     *
     * This function returns the audio file with the given name.
     * If the audio file does not exist, it returns an empty AudioFile.
     *
     * @param name The name of the audio file
     * @return The audio file
     */
    static types::audio_file_t get_audio_file(types::audio_name_t name);
    /**
     * @brief Get an audio stream
     *
     * This function returns the audio stream with the given name.
     * If the audio stream does not exist, it returns nullptr.
     *
     * @param name The name of the audio stream
     * @return The audio stream
     */
    static SDL_AudioStream *get_stream(types::stream_name_t name);

    /**
     * @brief Load an audio file
     *
     * This function loads an audio file from the given path and
     * stores it in the audiofiles map with the given name.
     *
     * @param name The name of the audio file
     * @param path The path to the audio file
     */
    static void load_audio(types::audio_name_t name, std::string path);
    /**
     * @brief Create an audio stream
     *
     * This function creates an audio stream with the given name.
     * If a stream with the same name already exists, it does nothing.
     *
     * @param name The name of the stream
     */
    static void create_stream(types::stream_name_t);
    /**
     * @brief Play an audio file
     *
     * This function plays the audio file with the given name on the
     * stream with the given name. If the stream does not exist, it
     * creates a new stream with the given name. If the audio file
     * does not exist, it does nothing.
     *
     * @param audio_name The name of the audio file
     * @param stream_name The name of the stream
     */
    static void play_audio(types::audio_name_t, types::stream_name_t = "default");
    /**
     * @brief Set the volume of a stream
     *
     * This function sets the volume of the stream with the given name.
     * If the stream does not exist, it does nothing.
     *
     * @param name The name of the stream
     * @param volume The volume of the stream
     */
    static void set_volume(types::stream_name_t name, int volume);
    /**
     * @brief Pause a stream
     *
     * This function pauses the stream with the given name.
     * If the stream does not exist, it does nothing.
     *
     * @param name The name of the stream
     */
    static void pause_stream(types::stream_name_t name);
    /**
     * @brief Resume a stream
     *
     * This function resumes the stream with the given name.
     * If the stream does not exist, it does nothing.
     *
     * @param name The name of the stream
     */
    static void resume_stream(types::stream_name_t name);
    /**
     * @brief Stop a stream
     *
     * This function stops the stream with the given name.
     * If the stream does not exist, it does nothing.
     *
     * @param name The name of the stream
     */
    static void clear_stream(types::stream_name_t name);

  private:
    static void check_error_audio();
};

} // namespace brenta
