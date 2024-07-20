#include "vao.h"

using namespace ECS::Types;

void VAO::Init()
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

void VAO::Unbind()
{
    glBindVertexArray(0);
}

void VAO::SetVertexData(Buffer buffer, unsigned int index, GLint size, GLenum type,
                    GLboolean normalized, GLsizei stride,
                    const void* pointer)
{
    Bind();
    buffer.Bind();
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    glEnableVertexAttribArray(index);
    buffer.Unbind();
    Unbind();
}

void VAO::Delete()
{
    glDeleteVertexArrays(1, &vao);
}
