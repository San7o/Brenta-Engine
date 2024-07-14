#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>       /* OpenGL driver */

/*
 * Vertex Array Object (VAO)
 *
 * It stores the format of the vertex data as well as
 * the Buffer Objects (see below) providing the vertex data arrays
 */
class VAO
{
public:
    unsigned int vao;
    VAO();
    unsigned int GetVAO();
    void Bind();
    /* Specifies the location and data format of the vertex attributes. */
    void SetVertexData(unsigned int index, GLint size, GLenum type,
                    GLboolean normalized, GLsizei stride,
                    const void* pointer);
    void Delete();
};
#endif
