// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/buffer.hpp>
#include <brenta/shader.hpp>
#include <brenta/vao.hpp>
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

namespace types
{

/**
 * @brief Character struct
 *
 * This struct is used to store the information of a character
 * that is used to render text on the screen. The struct contains
 * the texture ID, the size of the glyph, the bearing, and the
 * advance of the glyph.
 */
struct character
{
  unsigned int texture_id; // ID handle of the glyph texture
  glm::ivec2 size;         // Size of glyph
  glm::ivec2 bearing;      // Offset from baseline to left/top of glyph
  unsigned int advance;    // Offset to advance to next glyph
};

} // namespace types

/**
 * @brief Text subsystem
 *
 * This class is used to render text on the screen. The text is
 * rendered using the FreeType library to load the font and the
 * characters, and OpenGL to render the text on the screen.
 */
class text : public subsystem
{
protected:

  static std::string font_path;
  static int font_size;
  
public:
  
  /**
   * @brief Map of characters
   *
   * Map an ascii character to a Character struct
   */
  static std::map<char, types::character> characters;

  class builder;

  static const std::string subsystem_name;

  // Subsystem interface
  /**
   * @brief Initialize the text subsystem
   *
   * This method initializes the text subsystem by loading the shader,
   * the VAO, and the VBO. The shader is loaded from the file text.vs
   * and text.fs, and the VAO and VBO are created using the data from
   * the characters map.
   *
   * Note: opengl context must be created before calling this
   * method.
   */
  std::expected<void, subsystem::error> initialize() override;

  /**
   * @brief Cleaup resources
   */
  std::expected<void, subsystem::error> terminate() override;
  std::string name() override;

  // Constructors / destructors
  text() = default;
  ~text() = default;
  
  // Member functions
  
  static text &instance();

  /**
   * @brief Load a font
   *
   * This method loads a font from the given file and size.  The font
   * is loaded using the FreeType library, and the characters are
   * stored in the characters map.
   *
   * @param font_name Name of the font file
   * @param font_size The size of the font
   */
  static void load(std::string font_name, int font_size = 48);
  
  /**
   * @brief Render text
   *
   * This method renders the given text on the screen at the given
   * position, with the given scale and color. The text is rendered
   * using the shader, VAO, and VBO that are initialized in the Init
   * method.
   *
   * @param text Text to render
   * @param x X position of the text
   * @param y Y position of the text
   * @param scale Scale of the text
   * @param color Color of the text
   */
  static void render_text(std::string text, float x, float y, float scale,
                          glm::vec3 color);

private:
  
  static types::shader_name_t text_shader;
  static types::vao text_vao;
  static types::buffer text_vbo;
};

class text::builder : public subsystem::builder
{
private:

  std::string font_path = "examples/assets/fonts/arial.ttf";
  int font_size = 48;
  
public:

  builder() = default;
  ~builder() = default;

  builder &font(std::string font_path);
  builder &size(int font_size);
  
  brenta::subsystem &build() override;
  
};

  
} // namespace brenta
