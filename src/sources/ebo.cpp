#include "ebo.h"

EBO::EBO()
{
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ARRAY_BUFFER, ebo);
}

void EBO::CopyVertices(GLsizeiptr size, const void* data, GLenum usage)
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
}

void EBO::Delete()
{
    glDeleteBuffers(1, &ebo);
}
