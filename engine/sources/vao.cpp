#include "vao.h"

using namespace ECS::Types;

VAO::VAO()
{
    glGenVertexArrays(1, &vao);
    Bind();
}

unsigned int VAO::GetVAO()
{
    return vao;
}

void VAO::Bind()
{
    glBindVertexArray(vao);
}

void VAO::SetVertexData(unsigned int index, GLint size, GLenum type,
                    GLboolean normalized, GLsizei stride,
                    const void* pointer)
{
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    glEnableVertexAttribArray(index);
}

void VAO::Delete()
{
    glDeleteVertexArrays(1, &vao);
}
