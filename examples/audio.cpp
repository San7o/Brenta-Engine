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

/**
 * Just a simple window
 */

#include <iostream>
#include <filesystem>
#include "engine.hpp"

using namespace Brenta;

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;
const bool isMouseCaptured = false;

#define ABS(...) std::filesystem::absolute(__VA_ARGS__)

int main() {

    Engine engine = Engine::Builder()
            .use_screen(true)
            .use_audio(true)              /* Enable audio */
            .set_screen_width(SCR_WIDTH)
            .set_screen_height(SCR_HEIGHT)
            .set_screen_is_mouse_captured(isMouseCaptured)
            .build();
    
    /* Load an audio file, assign it the name "guitar" */
    Audio::LoadAudio("guitar", ABS("assets/audio/guitar.wav"));

    while(!Screen::isWindowClosed()) {

        if (Screen::isKeyPressed(GLFW_KEY_ESCAPE))
            Screen::SetClose();

        /* 
         * Press space to play the audio "guitar" in the 
         * default audio stream
         */
        if (Screen::isKeyPressed(GLFW_KEY_SPACE))
                Audio::PlayAudio("guitar");

        Screen::PollEvents();
        Screen::SwapBuffers();
    }

    return 0;
}
