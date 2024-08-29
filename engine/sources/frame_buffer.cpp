#include "frame_buffer.hpp"

#include "engine_logger.hpp"
#include "gl_helper.hpp"
#include "screen.hpp"

using namespace Brenta;
using namespace Brenta::Types;
using namespace Brenta::Utils;

FrameBuffer::FrameBuffer(int width, int height)
{
    glGenFramebuffers(1, &this->id);
    if (this->id == 0)
    {
        ERROR("Error creating framebuffer!");
        exit(1);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
    glCheckError();

    glGenTextures(1, &this->texture_id);
    if (this->texture_id == 0)
    {
        ERROR("Error creating texture!");
        exit(1);
    }
    glBindTexture(GL_TEXTURE_2D, this->texture_id);
    glCheckError();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
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
    glCheckError();

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        ERROR("Framebuffer is not complete!");
        exit(1);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &this->id);
    glDeleteTextures(1, &this->texture_id);
}

void FrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
    glCheckError();
}

void FrameBuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
}

void FrameBuffer::Delete()
{
    glDeleteFramebuffers(1, &this->id);
    glDeleteTextures(1, &this->texture_id);
}

void FrameBuffer::Rescale(int width, int height)
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        ERROR("Framebuffer is not complete!");
        return;
    }

    glBindTexture(GL_TEXTURE_2D, this->texture_id);
    glCheckError();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           this->texture_id, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glCheckError();

    glBindRenderbuffer(GL_RENDERBUFFER, this->render_buffer_id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, this->render_buffer_id);

    Screen::WIDTH = width;
    Screen::HEIGHT = height;
}
