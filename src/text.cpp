// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/logger.hpp>
#include <brenta/window.hpp>
#include <brenta/text.hpp>
#include <brenta/texture.hpp>

using namespace brenta;

//
// Static variables
//

std::string text::font_path;
int text::font_size;
types::shader_name_t text::text_shader;
types::vao text::text_vao;
types::buffer text::text_vbo;
std::map<char, types::character> text::characters;
const std::string text::subsystem_name = "text";

//
// Subsystem interface
//

std::expected<void, subsystem::error> text::initialize()
{
  text::text_vbo = types::buffer(GL_ARRAY_BUFFER);
  text::text_vao.init();
  if (text::font_path != "")
    load(text::font_path, text::font_size);

  INFO("{}: initialized", text::subsystem_name);
  return {};
}

std::expected<void, subsystem::error> text::terminate()
{
  INFO("{}: text terminated", text::subsystem_name);
  return {};
}

std::string text::name()
{
  return text::subsystem_name;
}

//
// Member functions
//

text &text::instance()
{
  static text _text;
  return _text;
}

void text::load(std::string font_path, int font_size)
{
  if (text_vao.get_vao() == 0)
  {
    ERROR("{}: not initialized", text::subsystem_name);
    return;
  }
  FT_Library ft;
  if (FT_Init_FreeType(&ft))
  {
    ERROR("{}: could not init FreeType library", text::subsystem_name);
    return;
  }

  shader::create("TextShader",
                 GL_VERTEX_SHADER, "src/shaders/text.vs",
                 GL_FRAGMENT_SHADER, "src/shaders/text.fs");
  text_shader = "TextShader";
  shader::use(text_shader);

  // find path to font
  std::string font_name = font_path;
  if (font_name.empty())
  {
    ERROR("{}: could not find font", text::subsystem_name);
    return;
  }

  FT_Face face;
  if (FT_New_Face(ft, font_name.c_str(), 0, &face))
  {
    ERROR("{}: could not load font", text::subsystem_name);
    return;
  }
  else
  {
    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, font_size);

    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // load first 128 characters of ASCII set
    for (unsigned char c = 0; c < 128; c++)
    {
      // Load character glyph
      if (FT_Load_Char(face, c, FT_LOAD_RENDER))
      {
        ERROR("{}: could not load glyph", text::subsystem_name);
        continue;
      }
      // generate texture
      unsigned int texture;
      glGenTextures(1, &texture);
      texture::bind_texture(GL_TEXTURE_2D, texture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                   face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                   face->glyph->bitmap.buffer);
      // set texture options
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      // now store character for later use
      types::character character_ = {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        static_cast<unsigned int>(face->glyph->advance.x)};
      characters.insert(std::pair<char, types::character>(c, character_));
    }
    texture::bind_texture(GL_TEXTURE_2D, 0);
  }
  // destroy FreeType once we're finished
  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  // configure VAO/VBO for texture quads
  text_vao.bind();
  text_vbo.bind();
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  text_vbo.unbind();
  text_vao.unbind();
}

void text::render_text(std::string text, float x, float y, float scale,
                       glm::vec3 color)
{
  if (text_vao.get_vao() == 0)
  {
    ERROR("{}: not initialized", text::subsystem_name);
    return;
  }

  shader::use(text_shader);
  unsigned int textShaderId = shader::get_id(text_shader);

  glUniform3f(glGetUniformLocation(textShaderId, "textColor"), color.x, color.y,
              color.z);

  glm::mat4 projection =
    glm::ortho(0.0f, static_cast<float>(window::get_width()), 0.0f,
               static_cast<float>(window::get_height()));
  glUniformMatrix4fv(glGetUniformLocation(textShaderId, "projection"), 1,
                     GL_FALSE, glm::value_ptr(projection));

  glActiveTexture(GL_TEXTURE0);
  text_vao.bind();

  // iterate through all characters
  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++)
  {
    types::character ch = characters[*c];

    float xpos = x + ch.bearing.x * scale;
    float ypos = y - (ch.size.y - ch.bearing.y) * scale;

    float w = ch.size.x * scale;
    float h = ch.size.y * scale;

    // update VBO for each character

    float vertices[6][4] = {
      {xpos, ypos + h, 0.0, 0.0},    {xpos, ypos, 0.0, 1.0},
      {xpos + w, ypos, 1.0, 1.0},

      {xpos, ypos + h, 0.0, 0.0},    {xpos + w, ypos, 1.0, 1.0},
      {xpos + w, ypos + h, 1.0, 0.0}};

    // render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.texture_id);

    // update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, text_vbo.id);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // now advance cursors for next glyph (note that advance is number
    // of 1/64 pixels)
    x += (ch.advance >> 6)
         * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

//
// Builder
//

text::builder &text::builder::font(std::string font_path)
{
  this->font_path = font_path;
  return *this;
}

text::builder &text::builder::size(int font_size)
{
  this->font_size = font_size;
  return *this;
}

subsystem &text::builder::build()
{
  text::font_path = this->font_path;
  text::font_size = this->font_size;
  return text::instance();
}
