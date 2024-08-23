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

/*
 * Helper functions to interact with OpenGL.
 */

#pragma once

#include <glad/glad.h> /* OpenGL driver */

#define glCheckError() glCheckError_(__FILE__, __LINE__)

namespace Brenta
{

/**
 * @brief OpenGL helper functions
 *
 * This class contains helper functions to interact with OpenGL.
 */
class GL
{
  public:
    /**
     * @brief Load OpenGL
     *
     * This function loads OpenGL and sets some default values.
     *
     * @param gl_blending    Enable blending
     * @param gl_cull_face   Enable face culling
     * @param gl_multisample Enable multisampling
     * @param gl_depth_test  Enable depth testing
     */
    static void LoadOpenGL(bool gl_blending = true, bool gl_cull_face = true,
                           bool gl_multisample = true,
                           bool gl_depth_test = true);
    /**
     * @brief Set Poligon Mode
     * @param enable Enable or disable poligon mode
     */
    static void SetPoligonMode(GLboolean enable);
    /**
     * @brief Set Viewport
     *
     * This function sets the viewport of the window.
     *
     * @param x          X coordinate of the viewport
     * @param y          Y coordinate of the viewport
     * @param SCR_WIDTH  Width of the viewport
     * @param SCR_HEIGHT Height of the viewport
     */
    static void SetViewport(int x, int y, int SCR_WIDTH, int SCR_HEIGHT);
    /**
     * @brief Set Clear Color
     *
     * This function sets the clear color of the window.
     *
     * @param r Red component of the clear color
     * @param g Green component of the clear color
     * @param b Blue component of the clear color
     * @param a Alpha component of the clear color
     */
    static void SetColor(float r, float g, float b, float a);
    /**
     * @brief Draw Arrays
     *
     * This function draws primitives from the array data.
     *
     * @param mode  Specifies what kind of primitives to render
     * @param first Specifies the starting index in the enabled arrays
     * @param count Specifies the number of indices to be rendered
     */
    static void DrawArrays(GLenum mode, int first, int count);
    /**
     * @brief Draw Elements
     *
     * This function draws primitives from the array data.
     *
     * @param mode    Specifies what kind of primitives to render
     * @param count   Specifies the number of elements to be rendered
     * @param type    Specifies the type of the values in indices
     * @param indices Specifies a pointer to the location where the indices are
     * stored
     */
    static void DrawElements(GLenum mode, int count, GLenum type,
                             const void *indices);
    /**
     * @brief Clear
     *
     * This function clears the color and depth buffer.
     */
    static void Clear();
    /**
     * @brief Enable Depth Test
     *
     * This function enables the depth test.
     */
    static void BindVertexArray(unsigned int n);
    /**
     * @brief Check OpenGL error
     *
     * This function checks if there is an OpenGL error.
     *
     * @return The error code
     */
    static GLenum glCheckError_(const char *file, int line);
};

} // namespace Brenta
