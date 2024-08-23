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

#include "buffer.hpp"
#include "camera.hpp"
#include "engine_audio.hpp"
#include "engine_input.hpp"
#include "engine_logger.hpp"
#include "engine_time.hpp"
#include "gl_helper.hpp"
#include "mesh.hpp"
#include "model.hpp"
#include "particles.hpp"
#include "screen.hpp"
#include "shader.hpp"
#include "text.hpp"
#include "texture.hpp"
#include "translation.hpp"
#include "vao.hpp"

namespace Brenta
{

/**
 * @brief Engine setup
 *
 * This class is used to setup every subsystem of the engine. You can
 * use the builder class to set the parameters of the engine: you can set
 * which subsystem to use and configure them.
 *
 * It automatically destroys the subsystems when the engine is destroyed.
 */
class Engine
{
  public:
    bool uses_screen;
    bool uses_audio;
    bool uses_input;
    bool uses_logger;
    bool uses_text;
    bool uses_ecs;
    int screen_width;
    int screen_height;
    bool screen_is_mouse_captured;
    const char *screen_title;
    bool screen_msaa;
    bool screen_vsync;
    Types::LogLevel log_level;
    std::string log_file;
    std::string text_font;
    int text_size;
    bool gl_blending;
    bool gl_cull_face;
    bool gl_multisample;
    bool gl_depth_test;

    Engine (bool uses_screen, bool uses_audio, bool uses_input,
            bool uses_logger, bool uses_text, bool uses_ecs, int screen_width,
            int screen_height, bool screen_is_mouse_captured, bool screen_msaa,
            bool screen_vsync, const char *screen_title,
            Types::LogLevel log_level, std::string log_file,
            std::string text_font, int text_size, bool gl_blending,
            bool gl_cull_face, bool gl_multisample, bool gl_depth_test);
    ~Engine ();

    class Builder;
};

/**
 * @brief Engine builder
 *
 * This class is used to build the engine. You can set the parameters of the
 * engine using the methods of this class and then call the build method to
 * create the engine.
 */
class Engine::Builder
{
  public:
    bool uses_screen = false;
    bool uses_audio = false;
    bool uses_input = false;
    bool uses_logger = false;
    bool uses_text = false;
    bool uses_ecs = false;
    int screen_width = 1280;
    int screen_height = 720;
    bool screen_is_mouse_captured = false;
    bool screen_msaa = false;
    bool screen_vsync = false;
    const char *screen_title = "";
    Types::LogLevel log_level = Types::LogLevel::WARNING;
    std::string log_file = "./logs/log.txt";
    std::string text_font = "arial.ttf";
    int text_size = 48;
    bool gl_blending = true;
    bool gl_cull_face = true;
    bool gl_multisample = true;
    bool gl_depth_test = true;

    Builder &use_screen (bool uses_screen);
    Builder &use_audio (bool uses_audio);
    Builder &use_input (bool uses_input);
    Builder &use_logger (bool uses_logger);
    Builder &use_text (bool uses_text);
    Builder &use_ecs (bool uses_ecs);
    Builder &set_screen_width (int screen_width);
    Builder &set_screen_height (int screen_height);
    Builder &set_screen_is_mouse_captured (bool screen_is_mouse_captured);
    Builder &set_screen_title (const char *screen_title);
    Builder &set_screen_msaa (bool screen_msaa);
    Builder &set_screen_vsync (bool screen_vsync);
    Builder &set_log_level (Types::LogLevel log_level);
    Builder &set_log_file (std::string log_file);
    Builder &set_text_font (std::string text_font);
    Builder &set_text_size (int text_size);
    Builder &set_gl_blending (bool gl_blending);
    Builder &set_gl_cull_face (bool gl_cull_face);
    Builder &set_gl_multisample (bool gl_multisample);
    Builder &set_gl_depth_test (bool gl_depth_test);

    Engine build ();
};

} // namespace Brenta

// Documnetation Main Page:

/*! \mainpage Home
 *
 * \section intro_sec Introduction
 *
 * Hi and welcome to the engine documentation. This is a simple 3D game engine
 * using the Entity Component System architecture. The engine is composed of
 * several modules, which you can browse here or look at the code.
 *
 * \section install_sec Installation
 *
 * TODO
 *
 */
