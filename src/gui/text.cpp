// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/logger.hpp>
#include <brenta/window.hpp>
#include <brenta/gui/text.hpp>
#include <brenta/renderer/opengl/texture.hpp>

using namespace brenta;

//
// Static variables
//

bool              Text::initialized = false;
Shader::Name      Text::shader_name;
Vao               Text::vao;
Buffer            Text::vbo;
const std::string Text::subsystem_name = "text";
Text::Config      Text::init_config = default_config;
std::map<char, Text::Character> Text::characters;
const Text::Config Text::default_config = {
  "examples/assets/fonts/arial.ttf",
  48,
};

//
// Subsystem interface
//

std::expected<void, Subsystem::Error> Text::initialize()
{
  if (this->is_initialized()) return {};
  
  Text::vbo.init(GL_ARRAY_BUFFER);
  Text::vao.init();
  Text::vao.bind();
  if (Text::init_config.font_path != "")
    load(Text::init_config.font_path, Text::init_config.font_size);

  Text::initialized = true;
  INFO("{}: initialized", Text::subsystem_name);
  return {};
}

std::expected<void, Subsystem::Error> Text::terminate()
{
  if (!this->is_initialized()) return {};

  Text::initialized = false;
  INFO("{}: text terminated", Text::subsystem_name);
  return {};
}

std::string Text::name()
{
  return Text::subsystem_name;
}

bool Text::is_initialized()
{
  return Text::initialized;
}

//
// Member functions
//

Text &Text::instance()
{
  static Text _text;
  return _text;
}

void Text::load(const std::filesystem::path &font_path, int font_size)
{
  if (Text::vao.get_id() == 0)
  {
    ERROR("{}: not initialized", Text::subsystem_name);
    return;
  }
  FT_Library ft;
  if (FT_Init_FreeType(&ft))
  {
    ERROR("{}: could not init FreeType library", Text::subsystem_name);
    return;
  }

  auto shader = Shader::create("TextShader",
                               Shader::Type::Vertex,   std::filesystem::path("src/renderer/shaders/text.vs"),
                               Shader::Type::Fragment, std::filesystem::path("src/renderer/shaders/text.fs"));
  if (!shader) return;
  
  Text::shader_name = "TextShader";
  shader->use();

  // find path to font
  if (font_path.empty())
  {
    ERROR("{}: could not find font at path {}",
          Text::subsystem_name, font_path.c_str());
    return;
  }

  FT_Face face;
  if (FT_New_Face(ft, font_path.c_str(), 0, &face))
  {
    ERROR("{}: could not load font at path {}",
          Text::subsystem_name, font_path.c_str());
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
        ERROR("{}: could not load glyph", Text::subsystem_name);
        continue;
      }
      // generate texture
      unsigned int texture;
      glGenTextures(1, &texture);
      Texture::bind_id(GL_TEXTURE_2D, texture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                   face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                   face->glyph->bitmap.buffer);
      // set texture options
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      // now store character for later use
      Text::Character character = {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        static_cast<unsigned int>(face->glyph->advance.x)};
      characters.insert(std::pair<char, Text::Character>(c, character));
    }
    Texture::bind_id(GL_TEXTURE_2D, 0);
  }
  // destroy FreeType once we're finished
  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  // configure VAO/VBO for texture quads
  Text::vao.bind();
  Text::vbo.bind();
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  Text::vbo.unbind();
  Text::vao.unbind();
}

void Text::render_text(std::string text, float x, float y, float scale,
                       glm::vec3 color)
{
  if (Text::vao.get_id() == 0)
  {
    ERROR("{}: not initialized", Text::subsystem_name);
    return;
  }

  auto shader = Shader::get_shader(Text::shader_name);
  if (!shader) return;
  
  unsigned int textShaderId = shader->get_id();
  shader->use();
  shader->set_float3("textColor", color.x, color.y, color.z);

  glm::mat4 projection =
    glm::ortho(0.0f, static_cast<float>(Window::get_width()), 0.0f,
               static_cast<float>(Window::get_height()));
  glUniformMatrix4fv(glGetUniformLocation(textShaderId, "projection"), 1,
                     GL_FALSE, glm::value_ptr(projection));

  glActiveTexture(GL_TEXTURE0);
  Text::vao.bind();

  // iterate through all characters
  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++)
  {
    Text::Character ch = characters[*c];

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
    glBindBuffer(GL_ARRAY_BUFFER, Text::vbo.id);
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
  return;
}

//
// Builder
//

Text::Builder &Text::Builder::font(const std::filesystem::path &font_path)
{
  this->conf.font_path = font_path;
  return *this;
}

Text::Builder &Text::Builder::size(int font_size)
{
  this->conf.font_size = font_size;
  return *this;
}

Subsystem &Text::Builder::build()
{
  Text::init_config = this->conf;
  return Text::instance();
}
