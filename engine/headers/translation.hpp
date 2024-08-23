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

#pragma once

#include "engine.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Brenta
{

namespace Types
{

/**
 * @brief Translation util class
 *
 * This class is used to store the view, projection, and model matrices
 * of an object. The matrices can be set using the setView, setProjection,
 * and setModel methods.
 */
class Translation
{
  public:
    /**
     * @brief Camera view matrix
     */
    glm::mat4 view;
    /**
     * @brief Camera projection matrix
     */
    glm::mat4 projection;
    /**
     * @brief Object position matrix
     */
    glm::mat4 model;

    /**
     * @brief Translation constructor
     *
     * This constructor initializes the view, projection, and model
     * matrices to the identity matrix.
     */
    Translation();
    /**
     * @brief Translation constructor
     *
     * This constructor initializes the view, projection, and model
     * matrices to the given matrices.
     * @param view The view matrix
     * @param projection The projection matrix
     * @param model The model matrix
     */
    Translation(glm::mat4 view, glm::mat4 projection, glm::mat4 model);

    /**
     * @brief Set the view matrix
     * @param view The view matrix
     */
    void setView(glm::mat4 view);
    /**
     * @brief Set the projection matrix
     * @param projection The projection matrix
     */
    void setProjection(glm::mat4 projection);
    /**
     * @brief Set the model matrix
     * @param model The model matrix
     */
    void setProjection(float fov, float near, float far);
    /**
     * @brief Set the model matrix
     * @param model The model matrix
     */
    void setModel(glm::mat4 model);
    /**
     * @brief Translate the object
     * @param translation The translation vector
     */
    void translate(glm::vec3 translation);
    /**
     * @brief Rotate the object
     * @param rotation The rotation vector
     */
    void rotate(glm::vec3 rotation);
    /**
     * @brief Scale the object
     * @param scale The scale factor
     */
    void scale(float scale);
    /**
     * @brief Set the shader
     * @param shader_name The shader name
     */
    void setShader(Types::ShaderName shader_name);
};

} // namespace Types

} // namespace Brenta
