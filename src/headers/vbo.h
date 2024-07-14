#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>       /* OpenGL driver */

/*
 * Vertex Buffer Object (VBO)
 *
 * Used to store multiple vertices in the GPU memory so that
 * the data can be sent to the GPU in one go.
 */
class VBO
{
public:
    unsigned int vbo;
    VBO();
    void CopyVertices(GLsizeiptr size, const void* data, GLenum usage);
    void Bind();
    void Delete();
};
#endif
