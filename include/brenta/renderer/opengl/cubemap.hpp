// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/opengl/texture.hpp>
#include <brenta/logger.hpp>

#include <stb_image.h>

namespace brenta
{

class Cubemap : public Texture
{
public:

  Cubemap(const tenno::vector<std::filesystem::path>& faces)
    : Texture(Cubemap::default_config)
  {
    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);

    stbi_set_flip_vertically_on_load(Cubemap::default_config.properties.flipped);
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); ++i)
    {
      unsigned char *data = stbi_load(faces[i].string().c_str(),
                                      &width,
                                      &height,
                                      &nrChannels,
                                      0);
      if (!data)
      {
        ERROR("Error loading cubemap face {}", faces[i].string());
        stbi_image_free(data);
        continue;
      }

      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                   0, GL_RGB, width, height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);

      this->memory    += 3 * width * height;
      Texture::memory += this->memory;
      
    }
  }

private:

  static const Texture::Config default_config;
  
};

} // namespace brenta
