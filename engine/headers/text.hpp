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

#include <ft2build.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <string>
#include FT_FREETYPE_H

#include "engine.hpp"

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
    glm::ivec2 size;        // Size of glyph
    glm::ivec2 bearing;     // Offset from baseline to left/top of glyph
    unsigned int advance;   // Offset to advance to next glyph
};

} // namespace Types

/**
 * @brief Text subsystem
 *
 * This class is used to render text on the screen. The text
 * is rendered using the FreeType library to load the font and
 * the characters, and OpenGL to render the text on the screen.
 */
class text
{
  public:
    /**
     * @brief Map of characters
     *
     * Map an ascii character to a Character struct
     */
    static std::map<char, types::character> characters;

    text() = delete;
    /**
     * @brief Initialize the text subsystem
     *
     * This method initializes the text subsystem by loading
     * the shader, the VAO, and the VBO. The shader is loaded
     * from the file text.vs and text.fs, and the VAO and VBO
     * are created using the data from the characters map.
     *
     * Note: opengl context must be created before calling this
     * method.
     */
    static void init();

    /**
     * @brief Load a font
     *
     * This method loads a font from the given file and size.
     * The font is loaded using the FreeType library, and the
     * characters are stored in the characters map.
     *
     * @param font_name Name of the font file
     * @param font_size Size of the font
     */
    static void load(std::string font_name, unsigned int font_size);
    /**
     * @brief Render text
     *
     * This method renders the given text on the screen at the
     * given position, with the given scale and color. The text
     * is rendered using the shader, VAO, and VBO that are
     * initialized in the Init method.
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

} // namespace Brenta
