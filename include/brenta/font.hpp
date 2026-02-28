// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/opengl/shader.hpp>
#include <brenta/renderer/opengl/vao.hpp>
#include <brenta/renderer/opengl/buffer.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>

namespace brenta
{

class Font
{
public:

  class Character
  {
  public:
    unsigned int texture_id; // ID handle of the glyph texture
    glm::ivec2   size;       // Size of glyph
    glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
    unsigned int advance;    // Offset to advance to next glyph
  };

  class Builder;
  
  tenno::shared_ptr<Shader>   shader;
  Vao                         vao;
  Buffer                      vbo;
  std::map<char, Character>   characters;
  
  Font() = default;
  Font(const std::filesystem::path &path, int size);
  Font(Font&&) = default;
  ~Font();

};

class Font::Builder
{
public:

  Builder& path(const std::filesystem::path& path);
  Builder& size(int size);

  // Add path to be watched for hot-reloading
  Builder &watch(const std::filesystem::path &path);
  
  Font build();
  tenno::vector<std::filesystem::path> get_watch_paths() const;
  
private:

  std::filesystem::path _path;
  int                   _size;
  
  tenno::vector<std::filesystem::path> watch_paths = {};
  
};  

} // namespave brenta
