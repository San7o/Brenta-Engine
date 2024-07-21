#include "vao.h"
#include "engine_logger.h"

using namespace ECS::Types;

void VAO::Init()
{
    glGenVertexArrays(1, &vao);
    Bind();
}

unsigned int VAO::GetVAO()
{
    if (vao == 0) {
        ECS::Logger::Log(Types::LogLevel::ERROR, "VAO not initialized");
        return 0;
    }
    return vao;
}

void VAO::Bind()
{
    if (vao == 0) {
        ECS::Logger::Log(Types::LogLevel::ERROR, "VAO not initialized");
        return;
    }
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
    if (vao == 0) {
        ECS::Logger::Log(Types::LogLevel::ERROR, "VAO not initialized");
        return;
    }
    glDeleteVertexArrays(1, &vao);
}
