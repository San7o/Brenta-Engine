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

#pragma once

#include <glad/glad.h>       /* OpenGL driver */

#include "buffer.hpp"

namespace Brenta
{

namespace Types
{

/**
 * @brief Vertex Array Object (VAO)
 *
 * Wrapper for OpenGL Vertex Array Objects
 */
class VAO
{
public:
    /**
     * @brief Vertex Array Object (VAO)
     */
    unsigned int vao;

    /**
     * @brief Empty Constructor
     *
     * Does nothing
     */
    VAO() {}
    /**
     * @brief Init Constructor
     *
     * Creates a new VAO
     */
    void Init();
    
    /**
     * @brief Get the VAO
     * @return The VAO
     */
    unsigned int GetVAO();
    /**
     * @brief Bind the VAO
     */
    void Bind();
    /**
     * @brief Unbind the VAO
     */
    void Unbind();
    /**
     * @brief Delete the VAO
     */
    void Delete();
    /**
     * @brief Set the vertex data
     *
     * @param buffer The buffer object
     * @param index The index of the vertex attribute
     * @param size The number of components per attribute
     * @param type The data type of each component
     * @param normalized Whether the data should be normalized
     * @param stride The byte offset between consecutive generic vertex attributes
     * @param pointer The offset of the first component of the first generic vertex attribute in the array
     */
    void SetVertexData(Buffer buffer, unsigned int index, GLint size, GLenum type,
                    GLboolean normalized, GLsizei stride,
                    const void* pointer);
};

} // namespace Types

} // namespace Brenta
