// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/audio.hpp>
#include <brenta/buffer.hpp>
#include <brenta/camera.hpp>
#include <brenta/frame_buffer.hpp>
#include <brenta/gl.hpp>
#include <brenta/gui.hpp>
#include <brenta/input.hpp>
#include <brenta/logger.hpp>
#include <brenta/mesh.hpp>
#include <brenta/model.hpp>
#include <brenta/particles.hpp>
#include <brenta/screen.hpp>
#include <brenta/shader.hpp>
#include <brenta/text.hpp>
#include <brenta/texture.hpp>
#include <brenta/time.hpp>
#include <brenta/translation.hpp>
#include <brenta/vao.hpp>

namespace brenta
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
class engine
{
public:
  bool uses_screen;
  bool uses_audio;
  bool uses_input;
  bool uses_logger;
  bool uses_text;
  int screen_width;
  int screen_height;
  bool screen_is_mouse_captured;
  const char *screen_title;
  bool screen_msaa;
  bool screen_vsync;
  oak::level log_level;
  std::string log_file;
  std::string text_font;
  int text_size;
  bool gl_blending;
  bool gl_cull_face;
  bool gl_multisample;
  bool gl_depth_test;

  engine(bool uses_screen, bool uses_audio, bool uses_input, bool uses_logger,
         bool uses_text, int screen_width, int screen_height,
         bool screen_is_mouse_captured, bool screen_msaa, bool screen_vsync,
         const char *screen_title, oak::level log_level, std::string log_file,
         std::string text_font, int text_size, bool gl_blending,
         bool gl_cull_face, bool gl_multisample, bool gl_depth_test);
  ~engine();

  class builder;
};

/**
 * @brief Engine builder
 *
 * This class is used to build the engine. You can set the parameters of the
 * engine using the methods of this class and then call the build method to
 * create the engine.
 */
class engine::builder
{
public:
  bool uses_screen = false;
  bool uses_audio = false;
  bool uses_input = false;
  bool uses_logger = false;
  bool uses_text = false;
  int screen_width = 1280;
  int screen_height = 720;
  bool screen_is_mouse_captured = false;
  bool screen_msaa = false;
  bool screen_vsync = false;
  const char *screen_title = "";
  oak::level log_level = oak::level::info;
  std::string log_file = "";
  std::string text_font = "arial.ttf";
  int text_size = 48;
  bool gl_blending = true;
  bool gl_cull_face = true;
  bool gl_multisample = true;
  bool gl_depth_test = true;

  builder &use_screen(bool uses_screen);
  builder &use_audio(bool uses_audio);
  builder &use_input(bool uses_input);
  builder &use_logger(bool uses_logger);
  builder &use_text(bool uses_text);
  builder &set_screen_width(int screen_width);
  builder &set_screen_height(int screen_height);
  builder &set_screen_is_mouse_captured(bool screen_is_mouse_captured);
  builder &set_screen_title(const char *screen_title);
  builder &set_screen_msaa(bool screen_msaa);
  builder &set_screen_vsync(bool screen_vsync);
  builder &set_log_level(oak::level log_level);
  builder &set_log_file(std::string log_file);
  builder &set_text_font(std::string text_font);
  builder &set_text_size(int text_size);
  builder &set_gl_blending(bool gl_blending);
  builder &set_gl_cull_face(bool gl_cull_face);
  builder &set_gl_multisample(bool gl_multisample);
  builder &set_gl_depth_test(bool gl_depth_test);

  engine build();
};

} // namespace brenta
