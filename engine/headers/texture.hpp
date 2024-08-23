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

#include <glad/glad.h> /* OpenGL driver */
#include <string>

namespace Brenta
{

/**
 * @brief Texture class
 *
 * This class is used to load and manage textures.
 */
class Texture
{
  public:
    /**
     * @brief Load a texture from a file
     *
     * This method loads a texture from a file and returns
     * the texture ID. The texture is loaded using the stb_image
     * library. The texture is loaded with the specified wrapping
     * and filtering modes. The texture can also have mipmaps.
     *
     * @param path Path to the texture file
     * @param wrapping Wrapping mode of the texture
     * @param filtering_min Filtering mode of the texture
     * @param filtering_mag Filtering mode of the texture
     * @param hasMipmap If the texture has mipmaps
     * @param mipmap_min Mipmap filtering mode of the texture
     * @param mipmap_mag Mipmap filtering mode of the texture
     * @param flip If the texture should be flipped
     * @return The texture ID
     */
    static unsigned int
    LoadTexture(std::string path, GLint wrapping = GL_REPEAT,
                GLint filtering_min = GL_NEAREST,
                GLint filtering_mag = GL_NEAREST, GLboolean hasMipmap = GL_TRUE,
                GLint mipmap_min = GL_LINEAR_MIPMAP_LINEAR,
                GLint mipmap_mag = GL_LINEAR, bool flip = true);
    /**
     * @brief Activate a texture unit
     *
     * This method activates a texture unit.
     */
    static void ActiveTexture(GLenum texture);
    /**
     * @brief Bind a texture
     *
     * This method binds a texture to a target. The texture
     * is bound with the specified wrapping and filtering modes.
     *
     * You need to bind the texture before using it in the shader.
     */
    static void BindTexture(GLenum target, unsigned int texture,
                            GLint wrapping = GL_REPEAT,
                            GLint filtering_min = GL_NEAREST,
                            GLint filtering_mag = GL_NEAREST,
                            GLboolean hasMipmap = GL_TRUE,
                            GLint mipmap_min = GL_LINEAR_MIPMAP_LINEAR,
                            GLint mipmap_mag = GL_LINEAR);

  private:
    static void SetTextureWrapping(GLint wrapping);
    static void SetTextureFiltering(GLint filtering_min, GLint filtering_mag);
    static void SetMipmap(GLboolean hasMipmap, GLint mipmap_min,
                          GLint mipmap_mag);
    static void ReadImage(const char *path, bool flip);
};

} // namespace Brenta
