#pragma once

#include "buffer.hpp"

namespace Brenta
{

namespace Types
{

/**
 * @brief FrameBuffer class
 *
 * This class contains the FrameBuffer functions,
 * It's a wrapper around OpenGL FrameBuffers, providing
 * functions to create, bind, unbind, delete and rescale
 * the FrameBuffer.
 */
class FrameBuffer : public Buffer
{
  public:
    /**
     * @brief Itexture ID
     */
    GLuint texture_id;
    /**
     * @brief Render Buffer ID
     */
    GLuint render_buffer_id;
    
    /**
     * @brief Empty constructor
     * Does nothing
     */
    FrameBuffer()
    {
    }
    /**
     * @brief Constructor
     * Creates a framebuffer with a texture and a render buffer
     * @param width Width of the framebuffer
     * @param height Height of the framebuffer
     */
    FrameBuffer(int width, int height);
    /**
     * @brief Destructor
     * Deletes the framebuffer and its texture
     */
    ~FrameBuffer();
    /**
     * @brief Bind the framebuffer
     */
    void Bind();
    /**
     * @brief Unbind the framebuffer
     */
    void Unbind();
    /**
     * @brief Delete the framebuffer and its texture
     */
    void Delete();
    void CopyData(GLsizeiptr size, const void *data, GLenum usage)
    {
    }

    void CopyVertices(GLsizeiptr size, const void *data, GLenum usage)
    {
    }
    void CopyIndices(GLsizeiptr size, const void *data, GLenum usage)
    {
    }
    /**
     * @brief Rescale the framebuffer
     * @param width New width of the framebuffer
     * @param height New height of the framebuffer
     */
    void Rescale(int width, int height);
};

} // namespace Types

} // namespace Brenta
