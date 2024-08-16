/*
 * Helper functions to interact with textures
 */

#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>       /* OpenGL driver */

namespace ECS {

class Texture {
public:
    static unsigned int LoadTexture(std::string path,
                    GLint wrapping = GL_REPEAT,
                    GLint filtering_min = GL_NEAREST,
                    GLint filtering_mag = GL_NEAREST,
                    GLboolean hasMipmap = GL_TRUE,
                    GLint mipmap_min = GL_LINEAR_MIPMAP_LINEAR,
                    GLint mipmap_mag = GL_LINEAR,
                    bool flip = true);
    static void ActiveTexture(GLenum texture);
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
    static void SetMipmap(GLboolean hasMipmap, GLint mipmap_min, GLint mipmap_mag);
    static void ReadImage(const char* path, bool flip);
};

} // namespace ECS

#endif
