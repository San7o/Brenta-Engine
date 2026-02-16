// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/opengl/buffer.hpp>
#include <brenta/renderer/opengl/shader.hpp>
#include <brenta/renderer/opengl/vao.hpp>
#include <brenta/renderer/color.hpp>
#include <brenta/subsystem.hpp>

#include <ft2build.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <string>

#include FT_FREETYPE_H

namespace brenta
{

/**
 * @brief Text subsystem
 *
 * This class is used to render text on the screen. The text is
 * rendered using the FreeType library to load the font and the
 * characters, and OpenGL to render the text on the screen.
 */
class Text : public Subsystem
{
public:

  class Character;
  class Config;
  class Builder;
  
  // Subsystem interface
  static const std::string subsystem_name;
  // Note: opengl context must be created before calling this method.
  std::expected<void, Subsystem::Error> initialize() override;
  std::expected<void, Subsystem::Error> terminate()  override;
  std::string  name() override;
  bool         is_initialized() override;
  static Text &instance();

  // Constructors / destructors
  Text()  = default;
  ~Text() = default;
  
  // Member functions

  static void load(const std::filesystem::path &font_path,
                   int font_size = 48);
  static void render_text(std::string text,
                          float x, float y,
                          float scale,
                          Color color);

private:
  
  static Shader::Name shader_name;
  static Vao          vao;
  static Buffer       vbo;
  static bool         initialized;
  // Map an ascii character to a Character struct
  static std::map<char, Character> characters;
  
  static Text::Config       init_config;
};

/**
 * @brief Character struct
 *
 * This struct is used to store the information of a character
 * that is used to render text on the screen. The struct contains
 * the texture ID, the size of the glyph, the bearing, and the
 * advance of the glyph.
 */
class Text::Character
{
public:
  unsigned int texture_id; // ID handle of the glyph texture
  glm::ivec2   size;       // Size of glyph
  glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
  unsigned int advance;    // Offset to advance to next glyph
};

class Text::Config
{
public:
  std::filesystem::path font_path = "examples/assets/fonts/arial.ttf";
  int font_size                   = 48;
};
  
class Text::Builder : public Subsystem::Builder
{
private:

  Text::Config conf = {};
  
public:

  Builder() = default;
  ~Builder() = default;

  Builder &font(const std::filesystem::path &font_path);
  Builder &size(int font_size);
  
  brenta::Subsystem &build() override;
  
};
  
} // namespace brenta
