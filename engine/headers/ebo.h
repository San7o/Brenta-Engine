#ifndef EBO_H
#define EBO_H

#ifndef __glad_h_
#include <glad/glad.h>       /* OpenGL driver */
#endif

/*
 * Element Buffer Object (EBO)
 *
 * Used to store indices so that the GPU knows which vertices
 * to draw and in which order.
 */

namespace ECS {

class EBO {
public:
    unsigned int ebo;
    EBO();
    void CopyIndices(GLsizeiptr size, const void* data, GLenum usage);
    void Delete();
};

} // namespace ECS

#endif
