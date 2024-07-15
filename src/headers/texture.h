#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>       /* OpenGL driver */

class Texture
{
public:
    static unsigned int LoadTexture(const char* path,
                    const char* directory,
                    GLint wrapping = GL_REPEAT,
                    GLint filtering_min = GL_NEAREST,
                    GLint filtering_mag = GL_LINEAR,
                    GLboolean hasMipmap = GL_TRUE,
                    GLint mipmap_min = GL_LINEAR_MIPMAP_LINEAR,
                    GLint mipmap_mag = GL_LINEAR,
                    bool flip = true);
    static void ActiveTexture(GLenum texture);
    static void BindTexture(GLenum target, unsigned int texture);
private:
    static void SetTextureWrapping(GLint wrapping);
    static void SetTextureFiltering(GLint filtering_min, GLint filtering_mag);
    static void SetMipmap(GLboolean hasMipmap, GLint mipmap_min, GLint mipmap_mag);
    static void ReadImage(const char* path, bool flip);
};
#endif
