#include "vbo.h"

VBO::VBO()
{
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void VBO::CopyVertices(GLsizeiptr size, const void* data, GLenum usage)
{
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

void VBO::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void VBO::Delete()
{
    glDeleteBuffers(1, &vbo);
}
