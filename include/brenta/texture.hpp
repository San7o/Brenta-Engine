// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <glad/glad.h>
#include <string>

namespace brenta
{

/**
 * @brief Texture class
 *
 * This class is used to load and manage textures.
 */
class texture
{
public:
  /**
   * @brief Load a texture from a file
   *
   * This method loads a texture from a file and returns
   * the texture ID. The texture is loaded using the stb_image
   * library.
   *
   * @param path Path to the texture file
   * @return The texture ID
   */
  static unsigned int load_texture(std::string path, bool flip = true);
  /**
   * @brief Activate a texture unit
   *
   * This method activates a texture unit.
   */
  static void active_texture(GLenum texture);
  /**
   * @brief Bind a texture
   *
   * This method binds a texture to a target. The texture
   * is bound with the specified wrapping and filtering modes.
   *
   * You need to bind the texture before using it in the shader.
   */
  static void bind_texture(GLenum target, unsigned int texture,
                           GLint wrapping = GL_REPEAT,
                           GLint filtering_min = GL_NEAREST,
                           GLint filtering_mag = GL_NEAREST,
                           GLboolean hasMipmap = GL_TRUE,
                           GLint mipmap_min = GL_LINEAR_MIPMAP_LINEAR,
                           GLint mipmap_mag = GL_LINEAR);

private:
  static void set_texture_wrapping(GLint wrapping);
  static void set_texture_filtering(GLint filtering_min, GLint filtering_mag);
  static void set_mipmap(GLboolean has_mipmap, GLint mipmap_min,
                         GLint mipmap_mag);
  static void read_image(const char *path, bool flip);
};

} // namespace brenta
