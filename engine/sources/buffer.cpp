/*
 * MIT License
 * 
 * Copyright (c) 2024 Giovanni Santini

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */ 

#include "buffer.hpp"
#include "engine_logger.hpp"

using namespace Brenta::Types;
using namespace Brenta::Utils;

Buffer::Buffer(GLenum input_target)
{
    this->target = input_target;
    glGenBuffers(1, &id);
    Bind();
}

void Buffer::CopyData(GLsizeiptr size, const void* data, GLenum usage)
{
    glBufferData(this->target, size, data, usage);
}

void Buffer::CopyIndices(GLsizeiptr size, const void* data, GLenum usage)
{
    if (this->target != GL_ELEMENT_ARRAY_BUFFER)
        return;
    Bind();
    glBufferData(this->target, size, data, usage);
}

void Buffer::CopyVertices(GLsizeiptr size, const void* data, GLenum usage)
{
    if (this->target == GL_ELEMENT_ARRAY_BUFFER)
        return;
    Bind();
    glBufferData(this->target, size, data, usage);
}

void Buffer::Bind()
{
    if (this->id == 0) {
        ERROR("Buffer not initialized");
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
        ERROR("Buffer not initialized");
        return;
    }
    glDeleteBuffers(1, &this->id);
}

int Buffer::GetId()
{
    return this->id;
}

GLenum Buffer::GetTarget()
{
    return this->target;
}

void Buffer::SetId(unsigned int id)
{
    this->id = id;
}

void Buffer::SetTarget(GLenum target)
{
    this->target = target;
}
