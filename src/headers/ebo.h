#ifndef EBO_H
#define EBO_H

#include <glad/glad.h>       /* OpenGL driver */

/*
 * Element Buffer Object (EBO)
 *
 * Used to store indices so that the GPU knows which vertices
 * to draw and in which order.
 */
class EBO
{
public:
    unsigned int ebo;
    EBO();
    void CopyVertices(GLsizeiptr size, const void* data, GLenum usage);
    void Delete();
};
#endif
