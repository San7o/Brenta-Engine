#pragma once

#ifndef BUFFER_H
#define BUFFER_H

#ifndef __glad_h_
#include <glad/glad.h>       /* OpenGL driver */
#endif

namespace ECS {

namespace Types {

class Buffer {
public:
    unsigned int id;
    GLenum target;

    Buffer() {}
    Buffer(GLenum input_target);

    void Bind();
    void Unbind();
    void Delete();

    /* Only for target = GL_ARRAY_BUFFER */
    void CopyVertices(GLsizeiptr size, const void* data, GLenum usage);
    /* Only for target = GL_ELEMENT_ARRAY_BUFFER */
    void CopyIndices(GLsizeiptr size, const void* data, GLenum usage);
};

} // namespace Types

} // namespace ECS

#endif
