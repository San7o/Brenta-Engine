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

#include "frame_buffer.hpp"

#include "engine_logger.hpp"
#include "gl_helper.hpp"
#include "screen.hpp"

using namespace brenta;
using namespace brenta::types;

framebuffer::framebuffer(int width, int height, GLenum format)
{
    this->format = format;

    glGenFramebuffers(1, &this->id);
    if (this->id == 0)
    {
        ERROR("Error creating framebuffer!");
        exit(1);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
    check_error();

    glGenTextures(1, &this->texture_id);
    if (this->texture_id == 0)
    {
        ERROR("Error creating texture!");
        exit(1);
    }
    glBindTexture(GL_TEXTURE_2D, this->texture_id);
    check_error();

    glTexImage2D(GL_TEXTURE_2D, 0, this->format, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           this->texture_id, 0);

    glGenRenderbuffers(1, &this->render_buffer_id);
    glBindRenderbuffer(GL_RENDERBUFFER, this->render_buffer_id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, this->render_buffer_id);
    check_error();

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        ERROR("Framebuffer is not complete!");
        exit(1);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

framebuffer::~framebuffer()
{
    glDeleteFramebuffers(1, &this->id);
    glDeleteTextures(1, &this->texture_id);
}

void framebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
    check_error();
}

void framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    check_error();
}

void framebuffer::destroy()
{
    glDeleteFramebuffers(1, &this->id);
    glDeleteTextures(1, &this->texture_id);
}

void framebuffer::rescale(int width, int height)
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        ERROR("Framebuffer is not complete!");
        return;
    }

    glBindTexture(GL_TEXTURE_2D, this->texture_id);
    check_error();

    glTexImage2D(GL_TEXTURE_2D, 0, this->format, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           this->texture_id, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    check_error();

    glBindRenderbuffer(GL_RENDERBUFFER, this->render_buffer_id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, this->render_buffer_id);

    screen::WIDTH = width;
    screen::HEIGHT = height;
}

void framebuffer::set_format(GLenum format)
{
    this->format = format;
}
