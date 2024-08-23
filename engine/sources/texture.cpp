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
 * The above copyright notice and this permission notice shall be included in all
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

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <stb_image.h>       /* Image loading library */
#include "texture.hpp"

using namespace Brenta;

unsigned int Texture::LoadTexture(std::string path,
                GLint wrapping, GLint filtering_min, GLint filtering_mag,
                GLboolean hasMipmap, GLint mipmap_min, GLint mipmap_mag,
                bool flip)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    //Texture::BindTexture(GL_TEXTURE_2D, texture, wrapping, filtering_min, filtering_mag, hasMipmap, mipmap_min, mipmap_mag);

    ReadImage(path.c_str(), flip);
    return texture;
}

void Texture::ActiveTexture(GLenum texture)
{
    glActiveTexture(texture);
}

void Texture::BindTexture(GLenum target, unsigned int texture,
                GLint wrapping, GLint filtering_min, GLint filtering_mag,
                GLboolean hasMipmap, GLint mipmap_min, GLint mipmap_mag)
{
    glBindTexture(target, texture);
    SetTextureWrapping(wrapping);
    SetTextureFiltering(filtering_min, filtering_mag);
    SetMipmap(hasMipmap, mipmap_min, mipmap_mag);
}

void Texture::SetTextureWrapping(GLint wrapping)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
}

void Texture::SetTextureFiltering(GLint filtering_min, GLint filtering_mag)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering_mag);
}

void Texture::SetMipmap(GLboolean hasMipmap, GLint mipmap_min, GLint mipmap_mag)
{
    if (hasMipmap)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap_min);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipmap_mag);
    }
}

void Texture::ReadImage(const char* path, bool flip)
{
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(flip);
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = GL_RGB;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture at location: " << path << std::endl;
    }
    stbi_image_free(data);
}
