#ifndef VBO_H
#define VBO_H

#ifndef __glad_h_
#include <glad/glad.h>       /* OpenGL driver */
#endif

/*
 * Vertex Buffer Object (VBO)
 *
 * Used to store multiple vertices in the GPU memory so that
 * the data can be sent to the GPU in one go.
 */

namespace ECS {

class VBO {
public:
    unsigned int vbo;
    VBO();
    void CopyVertices(GLsizeiptr size, const void* data, GLenum usage);
    void Bind();
    void Delete();
};

} // namespace ECS

#endif
