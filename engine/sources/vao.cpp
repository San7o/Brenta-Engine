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
 * The above copyright notice and this permission notice shall be included in
 all
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

#include "vao.hpp"

#include "engine_logger.hpp"

using namespace brenta::types;

void vao::init()
{
    glGenVertexArrays(1, &vao_id);
    bind();
}

unsigned int vao::get_vao()
{
    if (vao_id == 0)
    {
        ERROR("VAO not initialized");
        return 0;
    }
    return vao_id;
}

void vao::bind()
{
    if (this->get_vao() == 0)
    {
        ERROR("VAO not initialized");
        return;
    }
    glBindVertexArray(this->get_vao());
}

void vao::unbind()
{
    glBindVertexArray(0);
}

void vao::set_vertex_data(buffer buffer, unsigned int index, GLint size,
                        GLenum type, GLboolean normalized, GLsizei stride,
                        const void *pointer)
{
    bind();
    buffer.bind();
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    glEnableVertexAttribArray(index);
    buffer.unbind();
    unbind();
}

void vao::destroy()
{
    if (this->get_vao() == 0)
    {
        ERROR("VAO not initialized");
        return;
    }
    glDeleteVertexArrays(1, &this->vao_id);
}
