#include "buffer.h"
#include "engine_logger.h"

using namespace ECS::Types;

Buffer::Buffer(GLenum input_target)
{
    this->target = input_target;
    glGenBuffers(1, &id);
    Bind();
}

void Buffer::CopyIndices(GLsizeiptr size, const void* data, GLenum usage)
{
    if (this->target != GL_ELEMENT_ARRAY_BUFFER)
        return;
    glBufferData(this->target, size, data, usage);
}

void Buffer::CopyVertices(GLsizeiptr size, const void* data, GLenum usage)
{
    if (this->target == GL_ELEMENT_ARRAY_BUFFER)
        return;
    glBufferData(this->target, size, data, usage);
}

void Buffer::Bind()
{
    if (this->id == 0) {
        ECS::Logger::Log(LogLevel::ERROR, "Buffer not initialized");
        return;
    }
    glBindBuffer(this->target, this->id);
}

void Buffer::Unbind()
{
    glBindBuffer(this->target, 0);
}

void Buffer::Delete()
{
    if (this->id == 0) {
        ECS::Logger::Log(LogLevel::ERROR, "Buffer not initialized");
        return;
    }
    glDeleteBuffers(1, &this->id);
}
