// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/asset_manager.hpp>
#include <brenta/logger.hpp>

#include "renderer/shaders/c/text_fs.c"
#include "renderer/shaders/c/text_vs.c"

using namespace brenta;

Font::Font(const std::filesystem::path &path, int size)
{
  this->vao.init();
  this->vao.bind();
  this->vbo.init(Buffer::Target::Array);

  FT_Library ft;
  if (FT_Init_FreeType(&ft))
  {
    ERROR("Font: could not init FreeType library");
    return;
  }

  auto shader = AssetManager::get_shader("TextShader");
  if (!shader)
  {
    shader = AssetManager::new_shader("TextShader", {
        { Shader::Type::Vertex,   text_vs },
        { Shader::Type::Fragment, text_fs } });
  }
  if (!shader) return;
  
  this->shader = shader;
  this->shader->use();

  // find path to font
  if (path.empty())
  {
    ERROR("Font: could not find font at path {}", path.string());
    return;
  }

  FT_Face face;
  if (FT_New_Face(ft, path.string().c_str(), 0, &face))
  {
    ERROR("Font: could not load font at path {}", path.string());
    return;
  }
  else
  {
    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, size);

    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // load first 128 characters of ASCII set
    for (unsigned char c = 0; c < 128; c++)
    {
      // Load character glyph
      if (FT_Load_Char(face, c, FT_LOAD_RENDER))
      {
        ERROR("Font: could not load glyph for font at path {}",
              path.string());
        continue;
      }
      // generate texture
      unsigned int texture;
      glGenTextures(1, &texture);
      Texture::bind_id(Texture::Target::Texture2D, texture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                   face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                   face->glyph->bitmap.buffer);
      // set texture options
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      // now store character for later use
      Font::Character character = {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        static_cast<unsigned int>(face->glyph->advance.x)};
      characters.insert(std::pair<char, Font::Character>(c, character));
    }
    Texture::bind_id(Texture::Target::Texture2D, 0);
  }
  // destroy FreeType once we're finished
  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  // configure VAO/VBO for texture quads
  this->vao.bind();
  this->vbo.bind();
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  this->vbo.unbind();
  this->vao.unbind();
}

Font::~Font()
{
  this->shader = nullptr;
  this->vao.destroy();
  this->vbo.destroy();
}
