#ifndef VAO_H
#define VAO_H

#ifndef _glad_h_
#include <glad/glad.h>       /* OpenGL driver */
#endif

/*
 * Vertex Array Object (VAO)
 *
 * It stores the format of the vertex data as well as
 * the Buffer Objects (see below) providing the vertex data arrays
 */

namespace ECS {

namespace Types {

class VAO {
public:
    unsigned int vao;
    VAO();
    unsigned int GetVAO();
    void Bind();
    void Delete();

    /* Specifies the location and data format of the vertex attributes. */
    void SetVertexData(unsigned int index, GLint size, GLenum type,
                    GLboolean normalized, GLsizei stride,
                    const void* pointer);
};

} // namespace Types

} // namespace ECS

#endif
